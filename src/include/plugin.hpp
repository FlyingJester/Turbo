#pragma once
#include <jsapi.h>
#include <string>
#include <cassert>

namespace Turbo{

class Plugin{
    const char *name_;

    // This is the object holding the plugin. It is exposed to script under name()
    JS::Heap<JSObject *> obj_;
protected:
    // This is set by init.
    JSContext *ctx_;
    Plugin();
    Plugin(const char *a_name);

    JSContext *context(){ return ctx_; }

public:

    virtual ~Plugin(){}

    virtual const char *name();

    // This is guaranteed to be called before any other members.
    virtual void init(JSContext *ctx);

    virtual int numFunctions() = 0;
    virtual JSNative functionCallback(int i) = 0;
    virtual const char *functionName(int i) = 0;

    virtual int numVariables() = 0;
    virtual void variableValue(int i, JS::MutableHandleValue vp) = 0;
    virtual const char *variableName(int i) = 0;

    virtual void getObject(JS::MutableHandleObject obj){ obj.set(obj_); }

    virtual JSObject *getObject(){ return obj_.get(); }
    virtual const JSObject *getObject() const { return obj_.get(); }

    virtual JS::Heap<JSObject *> &getObjectHandle() { return obj_; }
    virtual const JS::Heap<JSObject *> &getObjectHandle() const { return obj_; }

    template<typename T>
    static bool setError(JSContext *ctx, const T &err_z){
        const std::string err = err_z;
        JS::RootedValue err_val(ctx, JS::StringValue(JS_NewStringCopyN(ctx, err.c_str(), err.length())));
        JS_SetPendingException(ctx, err_val);
        return false;
    }

    // Small helper for wrapping native objects.
    template<class T>
    static void wrapNativeObject(JSContext *ctx, T *that, const JSClass *clazz, JS::HandleObject prototype, JS::MutableHandle<JSObject*> aobj){
        JS::RootedObject global(ctx, JS::CurrentGlobalOrNull(ctx));
        aobj.set(JS_NewObjectWithGivenProto(ctx, clazz, prototype, global));
        JS_SetPrivate(aobj, that);
    }

    template<class T>
    static T *getSelfObject(JSContext *ctx, JS::Value *vp, JS::CallArgs *args, const JSClass *clazz){
        JS::RootedValue that_val(ctx, JS_THIS(ctx, vp));
        if((!that_val.isObject()) || that_val.isNull() || that_val.isUndefined())
            return nullptr;
        JS::RootedObject obj(ctx, that_val.toObjectOrNull());
        return static_cast<T *>(JS_GetInstancePrivate(ctx, obj, clazz, args));
    }
};

} // namespace Turbo
