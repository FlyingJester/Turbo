#include "sapphire.hpp"
#include "window.hpp"

namespace Sapphire {

JSClass SapphirePlugin::window_class = {
    "Window",
    JSCLASS_HAS_PRIVATE,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    Window::Finalizer, // finalizer
    nullptr,
    nullptr,
    Window::Constructor, // constructor
    nullptr,
    {nullptr}
};

static bool WindowWidth(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    const Window *const that = Turbo::Plugin::getSelfObject<Window>(ctx, vp, &args, &SapphirePlugin::window_class);
    if(!that)
        return Turbo::Plugin::setError(ctx, "WindowWidth Error: Invalid Window Object");

    args.rval().set(JS::RootedValue(ctx, JS::Int32Value(that->width())));
    return true;
}

static bool WindowHeight(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    const Window *const that = Turbo::Plugin::getSelfObject<Window>(ctx, vp, &args, &SapphirePlugin::window_class);
    if(!that)
        return Turbo::Plugin::setError(ctx, "WindowHeight Error: Invalid Window Object");

    args.rval().set(JS::RootedValue(ctx, JS::Int32Value(that->height())));
    return true;
}

static bool WindowShow(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    Window *const that = Turbo::Plugin::getSelfObject<Window>(ctx, vp, &args, &SapphirePlugin::window_class);
    if(!that)
        return Turbo::Plugin::setError(ctx, "WindowShow Error: Invalid Window Object");
    
    that->show();
    
    return true;
}

static bool WindowHide(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    Window *const that = Turbo::Plugin::getSelfObject<Window>(ctx, vp, &args, &SapphirePlugin::window_class);
    if(!that)
        return Turbo::Plugin::setError(ctx, "WindowHide Error: Invalid Window Object");
    
    that->hide();
    
    return true;
}

const std::array<JSFunctionSpec, 3> SapphirePlugin::window_methods = {{
    JS_FN("show", WindowShow, 1, 0),
    JS_FN("hide", WindowHide, 1, 0),
    JS_FS_END
}};

const std::array<JSPropertySpec, 3> SapphirePlugin::window_properties = {{
    JS_PSG("w", WindowWidth, 0),
    JS_PSG("h", WindowHeight, 0),
    JS_PS_END
}};

extern "C" TURBO_LIBRARY_EXPORT Turbo::Plugin *TurboPlugin(){
    return new SapphirePlugin();
}

} // namespace Sapphire
