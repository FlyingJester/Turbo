#include "window.hpp"
#include "sdl2_window.hpp"
#include <cassert>

namespace Sapphire{

Window::~Window(){

}

int Window::getScrolls(){
    int n = 0;

    do{
        n += getScroll();
    }while(areScrollsLeft());

    return n;
}

Window *Window::Create(uint64_t w, uint64_t h, const char *title){
    for(unsigned major = 4; major != 0; major--){
        for(unsigned minor = 4; minor != 0; minor--){
            if(Window *const win = Create(major, minor, w, h, title))
                return win;
        }
    }
    return nullptr;
}

Window *Window::Create(unsigned gl_major, unsigned gl_minor, uint64_t w, uint64_t h, const char *title){
    Window *win = nullptr;
#if SAPPHIRE_USE_SDL2
    if((win = new SDL2Window())){
        win->init(w, h, title);
        if(win->createGLContext(gl_major, gl_minor))
            return win;
        else{
            delete win;
            win = nullptr;
        }
    }
#endif
#if SAPPHIRE_USE_FLTK
    if((win = new FLTKWindow())){
        win->init(w, h, title);
        if(win->createGLContext(gl_major, gl_minor))
            return win;
        else{
            delete win;
            win = nullptr;
        }
    }
#endif
    assert(win == nullptr);

    return nullptr;
}

Window *Window::CreateNoGL(uint64_t w, uint64_t h, const char *title){
    Window *win = nullptr;

    if(false);
#if SAPPHIRE_USE_SDL2
    else if((win = new SDL2Window()));
#endif
#if SAPPHIRE_USE_FLTK
    else if((win = new FLTKWindow()));
#endif
    else
        return nullptr;

    if(win)
        win->init(w, h, title);

    return win;
}

} // namespace Sapphire
