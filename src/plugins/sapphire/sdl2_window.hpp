#pragma once
#include "window.hpp"

#if SAPPHIRE_USE_SDL2

#include <SDL2/SDL.h>

namespace Sapphire {

class SDL2Window : public Window{
    SDL_Window *window;
    SDL_GLContext context;
protected:
    bool init(uint64_t w, uint64_t h, const char *title) override;
public:
    SDL2Window(){}
    virtual ~SDL2Window();

    bool createGLContext(unsigned major, unsigned minor, Window *shared_with = nullptr) override;
    bool makeCurrentGL() const override;

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
