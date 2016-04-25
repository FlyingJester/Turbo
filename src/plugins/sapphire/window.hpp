#pragma once
#include "jsapi.h"
#include "sapphiresg/src/sapphire_sg.h"

namespace Sapphire {

class Window {
protected:
    struct SapphireSG_Context *sg_ctx;
    
    static void glInit();
    
public:
    virtual ~Window();

    static bool Constructor(JSContext *ctx, unsigned argc, JS::Value *vp);
    static void Finalizer(JSFreeOp *fop, JSObject *obj);

    virtual unsigned width() const = 0;
    virtual  unsigned height() const = 0;

    virtual void show() = 0;
    virtual void hide() = 0;

    inline struct SapphireSG_Context *getSGContext(){ return sg_ctx; }
};

} // namespace Sapphire
