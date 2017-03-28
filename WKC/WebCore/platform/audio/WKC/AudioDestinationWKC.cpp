/*
 * Copyright (c) 2012-2016 ACCESS CO., LTD. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if ENABLE(WEB_AUDIO)

#include "AudioDestination.h"
#include "AudioIOCallback.h"
#include "AudioSourceProviderClient.h"
#include "AudioBus.h"

#include "CurrentTime.h"
#include "Threading.h"

#include <wkc/wkcmediapeer.h>

namespace WebCore {

static const unsigned int c_busFrameSize = 128; // 3ms window
static const int c_drainBuffers = 3; // 9ms flush interval

class AudioDestinationWKC : public AudioDestination, AudioSourceProviderClient
{
    WTF_MAKE_FAST_ALLOCATED;
public:
    static AudioDestinationWKC* create(AudioIOCallback&, float sampleRate);

    // AudioDestination
    virtual ~AudioDestinationWKC();

    virtual void start();
    virtual void stop();
    virtual bool isPlaying();

    virtual float sampleRate() const;

    // AudioSourceProviderClient
    virtual void setFormat(size_t numberOfChannels, float sampleRate);

private:
    AudioDestinationWKC(AudioIOCallback&, float sampleRate);
    bool construct();
    static void threadProc(void* obj) { ((AudioDestinationWKC *)obj)->drain(); }
    void drain();

private:
    AudioIOCallback& m_audioIOCallback;
    RefPtr<AudioBus> m_bus;
    float m_sampleRate;
    size_t m_channels;

    void* m_peer;

    ThreadIdentifier m_thread;
    bool m_quit;

    float m_drainBuffer0[c_busFrameSize * c_drainBuffers];
    float m_drainBuffer1[c_busFrameSize * c_drainBuffers];
    int m_drainCount = 0;
};

AudioDestinationWKC::AudioDestinationWKC(AudioIOCallback& cb, float sampleRate)
    : m_audioIOCallback(cb)
    , m_bus(WebCore::AudioBus::create(2, c_busFrameSize, true))
    , m_sampleRate(sampleRate)
    , m_channels(2)
    , m_peer(0)
    , m_thread(0)
    , m_quit(false)
{
}

AudioDestinationWKC::~AudioDestinationWKC()
{
    if (m_thread) {
        m_quit = true;
        detachThread(m_thread);
    }

    if (m_peer)
        wkcAudioClosePeer(m_peer);
}

AudioDestinationWKC*
AudioDestinationWKC::create(AudioIOCallback& cb, float sampleRate)
{
    AudioDestinationWKC* self = 0;
    self = new AudioDestinationWKC(cb, sampleRate);
    if (!self->construct()) {
        delete self;
        return 0;
    }
    return self;
}

bool
AudioDestinationWKC::construct()
{
    m_peer = wkcAudioOpenPeer(wkcAudioPreferredSampleRatePeer(), 16, 2, 0);

    //Prevents crashing, but audio won't play if m_peer==nullptr
    return true;
}

void
AudioDestinationWKC::setFormat(size_t numberOfChannels, float sampleRate)
{
    m_channels = numberOfChannels;
    m_sampleRate = sampleRate;
}

void
AudioDestinationWKC::start()
{
    m_quit = false;
    m_thread = createThread(threadProc, this, "WKC: AudioDestination");
}

void
AudioDestinationWKC::stop()
{
    if (m_thread) {
        m_quit = true;

        // Wait for the audio thread to quit
        waitForThreadCompletion(m_thread);
        m_thread = 0;
    }
}

bool
AudioDestinationWKC::isPlaying()
{
    return m_peer ? true : false;
}

float
AudioDestinationWKC::sampleRate() const
{
    return m_sampleRate;
}

void AudioDestinationWKC::drain()
{
    if (m_quit)
        return;

    float* sampleMatrix[2] = { m_drainBuffer0, m_drainBuffer1 };
    const size_t sampleCount = c_busFrameSize * c_drainBuffers;
    const size_t len = sampleCount * 2 * sizeof(int16_t);
    int16_t* pcm = static_cast<int16_t*>(WTF::fastMalloc(len));

    while (!m_quit) {
        wkcThreadCheckAlivePeer();

        // Update on a 3ms window
        m_audioIOCallback.render(0, m_bus.get(), c_busFrameSize);

        if (m_peer) {
            int channels = 1;

            // Copy the samples into the drainBuffer
            float * sampleData0 = m_bus->channel(0)->mutableData();
            float * sampleData1 = sampleData0;
            channels = m_bus->numberOfChannels();

            if (channels > 1) {
                sampleData1 = m_bus->channel(1)->mutableData();
            }

            for (int i = 0; i < m_bus->channel(0)->length(); i++) {
                m_drainBuffer0[(m_drainCount*c_busFrameSize) + i] = sampleData0[i];
                m_drainBuffer1[(m_drainCount*c_busFrameSize) + i] = sampleData1[i];
            }

            m_drainCount++;

            // Flush on a 3ms * c_drainBuffers interval to compensate for HW flushing perf
            if (m_drainCount >= c_drainBuffers) {
                // Convert float to pcm16
                int16_t scaler = std::numeric_limits<int16_t>::max();
                float sample = 0.0f;
                for (auto i = 0; i < sampleCount; ++i) {
                    sample = sampleMatrix[0][i];

#ifdef ENABLE_SOFTWARE_CLAMPING
                    if (sample > 1.0f) {
                        sample = 1.0f;
                    } else if (sample < -1.0f) {
                        sample = -1.0f;
                    }
#endif
                    pcm[i * 2] = static_cast<int16_t>(scaler * sample);

                    sample = sampleMatrix[1][i];
#ifdef ENABLE_SOFTWARE_CLAMPING
                    if (sample > 1.0f) {
                        sample = 1.0f;
                    } else if (sample < -1.0f) {
                        sample = -1.0f;
                    }
#endif
                    pcm[i * 2 + 1] = static_cast<int16_t>(scaler * sample);
                }

                wkcAudioWritePeer(m_peer, pcm, len);
                m_drainCount = 0;
            }
        }

        wkc_usleep(1000);
    }

    WTF::fastFree(pcm);
}

std::unique_ptr<AudioDestination> AudioDestination::create(AudioIOCallback& cb, const String& inputDeviceId, unsigned numberOfInputChannels, unsigned numberOfOutputChannels, float sampleRate)
{
    return std::unique_ptr<AudioDestination>(AudioDestinationWKC::create(cb, sampleRate));
}

float AudioDestination::hardwareSampleRate()
{
    return wkcAudioPreferredSampleRatePeer();
}

unsigned long AudioDestination::maxChannelCount()
{
    return 0;
}

} // namespace WebCore

#endif // ENABLE(WEB_AUDIO)