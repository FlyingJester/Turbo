#pragma once

namespace Sapphire {

class Window{
public:
    Window() = delete;

    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void shown() = 0;

    virtual void flip() = 0;
    virtual void clear() = 0;

    virtual void h() = 0;
    virtual void w() = 0;

    virtual void getKey() = 0;
    virtual void areKeysLeft() = 0;
    virtual void clearKeys() = 0;

    virtual void getClick() = 0;
    virtual void areClicksLeft() = 0;
    virtual void clearClicks() = 0;

    virtual void getScroll() = 0;
    virtual void getScrolls() = 0;
    virtual void areScrollsLeft() = 0;
    virtual void clearScrolls() = 0;

    static Window *Create(int w, int h);

};

} // namespace Sapphire
