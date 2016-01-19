#pragma once
#include <jsapi.h>

namespace Turbo{

// The underlying object for a JS-side file.
class IO{
protected:

    IO();

public:
    
    virtual ~IO();
    
    virtual bool seekable() = 0;

    // Accepts SEEK_SET, SEEK_CUR, and SEEK_END
    virtual bool seek(unsigned offset, int whence) = 0;
    virtual int size() = 0;
    virtual int at() = 0;
    virtual bool readable() = 0;
    virtual bool writable() = 0;
    
    virtual bool read(void *to, unsigned n_bytes) = 0;
    virtual bool write(const void *from, unsigned n_bytes) = 0;

    virtual bool writeByte(unsigned char c){ return write(&c, 1); }
    virtual unsigned char readByte();

    // JS Interface
    static bool seekable(JSContext *ctx, unsigned argc, JS::Value *vp);
    static bool seek(JSContext *ctx, unsigned argc, JS::Value *vp);
    static bool read(JSContext *ctx, unsigned argc, JS::Value *vp);
    static bool write(JSContext *ctx, unsigned argc, JS::Value *vp);

    static bool setPosition(JSContext *ctx, unsigned argc, JS::Value *vp);
    static bool getPosition(JSContext *ctx, unsigned argc, JS::Value *vp);
    static bool size(JSContext *ctx, unsigned argc, JS::Value *vp);

    static void IOFinalizer(JSFreeOp *fop, JSObject *obj);

};

} // namespace Turbo
