#pragma once
#include "window.hpp"

#if SAPPHIRE_USE_SDL2

#include <SDL2/SDL.h>

namespace Sapphire {

class SDL2Window : public Window{
    SDL_Window *window;
public:
    SDL2Window();
    bool create(uint64_t w, uint64_t h, const char *title);

    void show() override;
    void hide() override;
    bool shown() const override;

    void flip() override;
    void clear() override;

    uint64_t h() const override;
    uint64_t w() const override;

    unsigned getKey() override;
    bool areKeysLeft() override;
    void clearKeys() override;

    unsigned getClick() override;
    bool areClicksLeft() override;
    void clearClicks() override;

    int getScroll() override;
    int getScrolls() override;
    bool areScrollsLeft() override;
    void clearScrolls() override;

};

} // namespace Sapphire

#endif
