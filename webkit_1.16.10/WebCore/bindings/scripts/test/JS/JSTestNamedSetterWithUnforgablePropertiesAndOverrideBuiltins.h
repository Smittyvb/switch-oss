/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#pragma once

#include "JSDOMWrapper.h"
#include "TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins.h"
#include <wtf/NeverDestroyed.h>

namespace WebCore {

class JSTestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins : public JSDOMWrapper<TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins> {
public:
    using Base = JSDOMWrapper<TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins>;
    static JSTestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, Ref<TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins>&& impl)
    {
        JSTestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins* ptr = new (NotNull, JSC::allocateCell<JSTestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins>(globalObject->vm().heap)) JSTestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins(structure, *globalObject, WTFMove(impl));
        ptr->finishCreation(globalObject->vm());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::VM&, JSDOMGlobalObject&);
    static JSC::JSObject* prototype(JSC::VM&, JSDOMGlobalObject&);
    static TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins* toWrapped(JSC::VM&, JSC::JSValue);
    static bool getOwnPropertySlot(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertySlotByIndex(JSC::JSObject*, JSC::ExecState*, unsigned propertyName, JSC::PropertySlot&);
    static void getOwnPropertyNames(JSC::JSObject*, JSC::ExecState*, JSC::PropertyNameArray&, JSC::EnumerationMode = JSC::EnumerationMode());
    static bool put(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    static bool putByIndex(JSC::JSCell*, JSC::ExecState*, unsigned propertyName, JSC::JSValue, bool shouldThrow);
    static bool defineOwnProperty(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, const JSC::PropertyDescriptor&, bool shouldThrow);
    static void destroy(JSC::JSCell*);

    DECLARE_INFO;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info(), JSC::MayHaveIndexedAccessors);
    }

    static JSC::JSValue getConstructor(JSC::VM&, const JSC::JSGlobalObject*);
public:
    static const unsigned StructureFlags = JSC::GetOwnPropertySlotIsImpure | JSC::HasStaticPropertyTable | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | JSC::OverridesGetOwnPropertySlot | JSC::OverridesGetPropertyNames | Base::StructureFlags | JSC::ProhibitsPropertyCaching;
protected:
    JSTestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins(JSC::Structure*, JSDOMGlobalObject&, Ref<TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins>&&);

    void finishCreation(JSC::VM&);
};

class JSTestNamedSetterWithUnforgablePropertiesAndOverrideBuiltinsOwner : public JSC::WeakHandleOwner {
public:
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld&, TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins*)
{
    static NeverDestroyed<JSTestNamedSetterWithUnforgablePropertiesAndOverrideBuiltinsOwner> owner;
#if PLATFORM(WKC)
    if (owner.isNull())
        owner.construct();
#endif
    return &owner.get();
}

inline void* wrapperKey(TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins* wrappableObject)
{
    return wrappableObject;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins&);
inline JSC::JSValue toJS(JSC::ExecState* state, JSDOMGlobalObject* globalObject, TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins* impl) { return impl ? toJS(state, globalObject, *impl) : JSC::jsNull(); }
JSC::JSValue toJSNewlyCreated(JSC::ExecState*, JSDOMGlobalObject*, Ref<TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins>&&);
inline JSC::JSValue toJSNewlyCreated(JSC::ExecState* state, JSDOMGlobalObject* globalObject, RefPtr<TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins>&& impl) { return impl ? toJSNewlyCreated(state, globalObject, impl.releaseNonNull()) : JSC::jsNull(); }

template<> struct JSDOMWrapperConverterTraits<TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins> {
    using WrapperClass = JSTestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins;
    using ToWrappedReturnType = TestNamedSetterWithUnforgablePropertiesAndOverrideBuiltins*;
};

} // namespace WebCore