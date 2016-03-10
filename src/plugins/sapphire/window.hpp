#pragma once
#include "plugin.hpp"
#include <cstdint>

namespace Sapphire {

class Window{
public:

    virtual void show() = 0;
    virtual void hide() = 0;
    virtual bool shown() const = 0;

    virtual void flip() = 0;
    virtual void clear() = 0;

    virtual uint64_t h() const = 0;
    virtual uint64_t w() const = 0;

    virtual unsigned getKey() = 0;
    virtual bool areKeysLeft() = 0;
    virtual void clearKeys() = 0;

    virtual unsigned getClick() = 0;
    virtual bool areClicksLeft() = 0;
    virtual void clearClicks() = 0;

    virtual int getScroll() = 0;
    virtual int getScrolls() = 0;
    virtual bool areScrollsLeft() = 0;
    virtual void clearScrolls() = 0;

    static Window *Create(uint64_t w, uint64_t h, const char *title = nullptr);

    static bool Constructor(JSContext *cx, unsigned argc, JS::Value *vp);
    static void Finalizer(JSFreeOp *fop, JSObject *obj);

};

} // namespace Sapphire
