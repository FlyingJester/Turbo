#pragma once
#include "window.hpp"

#if SAPPHIRE_USE_SDL2
#include <SDL2/SDL.h>

namespace Sapphire {

class SDL2Window : public Window{
    SDL_Window *window;
	SDL_GLContext gl_ctx;
    
    static void setSDL2GLProperties();
    
public:
    
    SDL2Window(unsigned w, unsigned h, const char *title);
    virtual ~SDL2Window();
    
    unsigned width() const override;
    unsigned height() const override;

    void show() override;
    void hide() override;
    
};

} // namespace Sapphire

#endif
