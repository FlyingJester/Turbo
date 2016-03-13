#include "sdl2_window.hpp"
#include <cassert>

#if SAPPHIRE_USE_SDL2

namespace Sapphire {

static std::array<unsigned, SAPPHIRE_NUM_KEYS> key_numbers;

static int16_t KeyFromSDLKey(unsigned sdl_key){
    for(size_t i = 0; i<key_numbers.size(); i++){
        if(key_numbers[i]==sdl_key)
            return i;
    }
    return -1;
}

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

void SDL2Window::clear() override;

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

static std::array<unsigned, NUM_KEYS> key_numbers = {{ // Translates directly to an SDL key constant
    SDLK_ESCAPE,
    SDLK_F1,
    SDLK_F2,
    SDLK_F3,
    SDLK_F4,
    SDLK_F5,
    SDLK_F6,
    SDLK_F7,
    SDLK_F8,
    SDLK_F9,
    SDLK_F10,
    SDLK_F11,
    SDLK_F12,
    SDLK_BACKQUOTE,      
    SDLK_0,
    SDLK_1, 
    SDLK_2, 
    SDLK_3, 
    SDLK_4, 
    SDLK_5, 
    SDLK_6, 
    SDLK_7, 
    SDLK_8, 
    SDLK_9, 
    SDLK_MINUS,
    SDLK_EQUALS,
    SDLK_BACKSPACE,
    SDLK_TAB,
    SDLK_a, 
    SDLK_b, 
    SDLK_c, 
    SDLK_d, 
    SDLK_e, 
    SDLK_f, 
    SDLK_g, 
    SDLK_h, 
    SDLK_i, 
    SDLK_j, 
    SDLK_k, 
    SDLK_l, 
    SDLK_m, 
    SDLK_n, 
    SDLK_o, 
    SDLK_p, 
    SDLK_q, 
    SDLK_r, 
    SDLK_s, 
    SDLK_t, 
    SDLK_u, 
    SDLK_v, 
    SDLK_w, 
    SDLK_x, 
    SDLK_y, 
    SDLK_z, 
    SDLK_LSHIFT,
    SDLK_RSHIFT,
    SDLK_LCTRL,
    SDLK_RCTRL,
    SDLK_LALT,
    SDLK_RALT,
    SDLK_SPACE,
    SDLK_LEFTBRACKET,
    SDLK_RIGHTBRACKET, 
    SDLK_SEMICOLON,
    SDLK_QUOTE,
    SDLK_COMMA,
    SDLK_PERIOD,
    SDLK_SLASH,
    SDLK_BACKSLASH,
    SDLK_RETURN,
    SDLK_RETURN,
    SDLK_INSERT,
    SDLK_DELETE,
    SDLK_HOME,
    SDLK_END,        
    SDLK_PAGEUP,     
    SDLK_PAGEDOWN, 
    SDLK_UP,
    SDLK_RIGHT,
    SDLK_DOWN,
    SDLK_LEFT,
    SDLK_KP_0,
    SDLK_KP_1,
    SDLK_KP_2,
    SDLK_KP_3,
    SDLK_KP_4,
    SDLK_KP_5,
    SDLK_KP_6,
    SDLK_KP_7,
    SDLK_KP_8,
    SDLK_KP_9,
    SDLK_CAPSLOCK,
    SDLK_NUMLOCKCLEAR,
    SDLK_SCROLLLOCK,
    SDLK_SCROLLLOCK,
}};

} // namespace Sapphire

#endif
