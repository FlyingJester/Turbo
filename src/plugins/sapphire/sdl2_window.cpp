#include "sdl2_window.hpp"
#include <cassert>

#if SAPPHIRE_USE_SDL2

SDL2Window::~SDL2Window(){
    if(context)
        SDL_GL_DeleteContext(context);
    if(window)
        SDL_DestroyWindow(window);
}

bool SDL2Window::init(uint64_t w, uint64_t h, const char *title){
    window = SDL_CreateWindow(title, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        w, h, SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI);
}

bool SDL2Window::createGLContext(unsigned major, unsigned minor, Window *shared_with = nullptr){
    assert(window);
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    context = SDL_GL_CreateContext(window);

    int l_major = 0, l_minor = 0, l_profile = 0;

    // Check that we got the correct profile and GL versions.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &l_major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &l_minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &l_profile);

    if( static_cast<int>(major) != l_major ||
        static_cast<int>(minor) != l_minor){
        fprintf(stderr, "[Sapphire] Error: Requested OpenGL %i.%i (Core), got OpenGL %i.%i\n",
            major, minor, l_major, l_minor);
    }

    return context;
}

bool SDL2Window::makeCurrentGL() const {
    assert(context);
    SDL_GL_MakeCurrent(window, context);
}

void SDL2Window::show() override;
void SDL2Window::hide() override;
bool SDL2Window::shown() const override;

void SDL2Window::flip(){
    assert(window);
    assert(context);
    SDL_GL_SwapWindow(window);
}

void SDL2Window::clear() override;

uint64_t SDL2Window::h() const override;
uint64_t SDL2Window::w() const override;

unsigned SDL2Window::getKey() override;
bool SDL2Window::areKeysLeft() override;
void SDL2Window::clearKeys() override;

unsigned SDL2Window::getClick() override;
bool SDL2Window::areClicksLeft() override;
void SDL2Window::clearClicks() override;

int SDL2Window::getScroll() override;
int SDL2Window::getScrolls() override;
bool SDL2Window::areScrollsLeft() override;
void SDL2Window::clearScrolls() override;


#endif
