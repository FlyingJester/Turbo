#pragma once
#include "plugin.hpp"
#include "keys.hpp"
#include <cstdint>

namespace Sapphire {

/*
 * TODO:
 *    - Should Window::are * Left() be const?
 *
 *
 */

class Window{
protected:
    virtual bool init(uint64_t w, uint64_t h, const char *title) = 0;
public:

    enum MouseButtons { eLeftButton, eRightButton, eMiddleButton, eOtherButton };

    virtual ~Window();

    virtual bool createGLContext(unsigned major, unsigned minor, Window *shared_with = nullptr) = 0;
    
    virtual bool makeCurrentGL() const = 0;

    virtual void show() = 0;
    virtual void hide() = 0;
    virtual bool shown() const = 0;

    virtual void flip() = 0;

    virtual uint64_t h() const = 0;
    virtual uint64_t w() const = 0;

    // Returns the next key press from the event queue. Will block if there are no presses in the queue.
    virtual KeyCode getKey() = 0;
    virtual bool areKeysLeft() = 0;
    virtual void clearKeys() = 0;
    virtual bool isKeyPressed(KeyCode) = 0;

    // Returns the next mouse click from the event queue. Will block if there are no clicks in the queue.
    virtual MouseButtons getClick() = 0;
    // Returns if there are any clicks in the event queue.
    virtual bool areClicksLeft() = 0;
    virtual void clearClicks() = 0;

    // value is the next scroll, either 1 for an up scroll or -1 for a down scroll
    virtual int getScroll() = 0;
    // Sums all incoming scrolls.
    // This will clear the scrolling state, but will block for another scroll if none exist in the queue.
    virtual int getScrolls();
    virtual bool areScrollsLeft() = 0;
    virtual void clearScrolls() = 0;

    static Window *Create(uint64_t w, uint64_t h, const char *title = nullptr);
    static Window *Create(unsigned gl_major, unsigned gl_minor, uint64_t w, uint64_t h, const char *title = nullptr);

    static Window *CreateNoGL(uint64_t w, uint64_t h, const char *title = nullptr);

    static bool Constructor(JSContext *cx, unsigned argc, JS::Value *vp);
    static void Finalizer(JSFreeOp *fop, JSObject *obj);

};

} // namespace Sapphire
