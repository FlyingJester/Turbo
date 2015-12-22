#pragma once
#include <jsapi.h>

namespace Ultra{

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
    virtual void setObject(JSObject *obj){ obj_ = obj; };
    virtual void setObject(JS::HandleObject obj){ obj_ = obj; }

};

} // namespace Ultra
