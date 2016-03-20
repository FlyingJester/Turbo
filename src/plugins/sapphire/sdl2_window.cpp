#include "sdl2_window.hpp"
#include "keys.hpp"
#include "sdl2_keys.hpp"
#include <array>
#include <cassert>

#if SAPPHIRE_USE_SDL2

namespace Sapphire {
/*
static int16_t KeyFromSDLKey(unsigned sdl_key){
    for(size_t i = 0; i<key_numbers.size(); i++){
        if(key_numbers[i]==sdl_key)
            return i;
    }
    return -1;
}
*/
SDL2Window::~SDL2Window(){
    if(m_context)
        SDL_GL_DeleteContext(m_context);
    if(m_window)
        SDL_DestroyWindow(m_window);
}

bool SDL2Window::init(uint64_t w, uint64_t h, const char *title){
    m_window = SDL_CreateWindow(title, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        w, h, SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI);
    return m_window;
}

bool SDL2Window::createGLContext(unsigned major, unsigned minor, Window *shared_with){
    assert(m_window);
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, shared_with!=nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_context = SDL_GL_CreateContext(m_window);

    makeCurrentGL();

    int l_major = 0, l_minor = 0, l_profile = 0;

    // Check that we got the correct profile and GL versions.
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &l_major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &l_minor);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &l_profile);

    if( static_cast<int>(major) != l_major ||
        static_cast<int>(minor) != l_minor){
        fprintf(stderr, "[Sapphire] Error: Requested OpenGL %i.%i (Core), got OpenGL %i.%i\n",
            major, minor, l_major, l_minor);
    }

    return m_context;
}

bool SDL2Window::makeCurrentGL() const {
    assert(m_context);
    SDL_GL_MakeCurrent(m_window, m_context);
    return true;
}

void SDL2Window::show(){
    assert(m_window);
    SDL_ShowWindow(m_window);
    m_shown = true;
}

void SDL2Window::hide(){
    assert(m_window);
    SDL_HideWindow(m_window);
    m_shown = false;
}

bool SDL2Window::shown() const{
    return m_shown;
}

void SDL2Window::flip(){
    assert(m_window);
    assert(m_context);
    SDL_GL_SwapWindow(m_window);
}

uint64_t SDL2Window::h() const{
    int h = 0;
    SDL_GetWindowSize(m_window, nullptr, &h);
    return h;
}

uint64_t SDL2Window::w() const{
    int w = 0;
    SDL_GetWindowSize(m_window, &w, nullptr);
    return w;
}

unsigned SDL2Window::getKey(){
    return 1;
}

bool SDL2Window::areKeysLeft() {
    return false;
}

void SDL2Window::clearKeys(){

}

bool SDL2Window::isKeyPressed(KeyCode k){
    return false;
}

Window::MouseButtons SDL2Window::getClick(){
    return eLeftButton;
}

bool SDL2Window::areClicksLeft(){
    return false;
}

void SDL2Window::clearClicks(){

}

int SDL2Window::getScroll(){
    return 0;
}

bool SDL2Window::areScrollsLeft(){
    return false;
}

void SDL2Window::clearScrolls(){

}

} // namespace Sapphire

#endif
