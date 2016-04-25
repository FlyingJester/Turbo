#include "window.hpp"
#include "plugin.hpp"
#include "sdl2_window.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>

namespace Sapphire {

using Turbo::Plugin;

void Window::glInit(){

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0, 0, 0, 0xFF);
}

Window::~Window(){}

bool Window::Constructor(JSContext *ctx, unsigned argc, JS::Value *vp){

    JS::CallArgs args = CallArgsFromVp(argc, vp);
    
    if(args.length()<2)
        return Plugin::setError(ctx, "Window::Constructor Error: requires two arguments");
    if(!args[0].isNumber())
        return Plugin::setError(ctx, "Window::Constructor Error: argument 0 must be a number");
    if(!args[1].isNumber())
        return Plugin::setError(ctx, "Window::Constructor Error: argument 1 must be a number");
    
    const int w = args[0].toInt32(), h = args[1].toInt32();
    
    if(w<=0)
        return Plugin::setError(ctx, "Window::Constructor Error: Width must be positive");
    if(h<=0)
        return Plugin::setError(ctx, "Window::Constructor Error: Height must be positive");
    
    const char *title = "Sapphire";
    //TODO: Window Title
    
    Window *win;
    
#if SAPPHIRE_USE_SDL2
    win = new SDL2Window(w, h, title); 
#endif
}

void Window::Finalizer(JSFreeOp *fop, JSObject *obj){

}

} // namespace Sapphire
