#include "sdl2_window.hpp"

#if SAPPHIRE_USE_SDL2
#include <SDL2/SDL.h>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>

namespace Sapphire {

static bool sdl2_was_init = false;

void SDL2Window::setSDL2GLProperties(){
    SDL_GL_ResetAttributes();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

}

SDL2Window::SDL2Window(unsigned w, unsigned h, const char *title){

    if(!sdl2_was_init){
        sdl2_was_init = true;
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    }
    
    setSDL2GLProperties();
    
	window = SDL_CreateWindow("SDL2 GL SapphireSG example",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS);
    
	gl_ctx = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_ctx);
    
	sg_ctx = SG_CreateContext(SG_OpenGL2, 2, 0);

	SG_SetCoordinateDistance(sg_ctx, w, h);

	glClearColor(0, 0, 0, 0xFF);

	SDL_GL_ResetAttributes();
}

SDL2Window::~SDL2Window(){
    
}

unsigned SDL2Window::width() const {
    int w;
    SDL_GetWindowSize(window, &w, nullptr);
    return w;
}

unsigned SDL2Window::height() const {
    int h;
    SDL_GetWindowSize(window, nullptr, &h);
    return h;
}

void SDL2Window::show(){
    SDL_ShowWindow(window);
}

void SDL2Window::hide(){
    SDL_HideWindow(window);
}

} // namespace Sapphire

#endif
