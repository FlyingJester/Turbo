#include "io.hpp"
#include "io_plugin.hpp"

namespace Turbo{

IO::IO(){}
IO::~IO(){}

unsigned char IO::readByte(){
    char c; 
    if(!read(&c, 1)) 
        return 0; 
    else 
        return c;
}

bool IO::seekable(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    IO *that = Plugin::getSelfObject<IO>(ctx, vp, &args, &IOPlugin::io_class);
    args.rval().set(JS::BooleanValue(that->seekable()));
    return true;
}

bool IO::seek(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    IO *that = Plugin::getSelfObject<IO>(ctx, vp, &args, &IOPlugin::io_class);
    if(!that)
        return Plugin::setError(ctx, "IO::seek Error: Invalid IO Object");

    if(!that->seekable())
        return Plugin::setError(ctx, "IO::seek Error: IO Object is not seekable");

    if(args.length()<2)
        return Plugin::setError(ctx, "IO::seek Error: requires two arguments");

    if(!args[0].isNumber())
        return Plugin::setError(ctx, "IO::seek Error: argument 0 must be a number.");

    if(!args[1].isInt32() || args[1].toInt32() != SEEK_CUR || args[1].toInt32() != SEEK_END || args[1].toInt32() != SEEK_SET )
        return Plugin::setError(ctx, "IO::seek Error: arument 1 is an invalid origin, must be seek_cur, seek_set, or seek_end");

    if(!that->seek(args[0].toNumber(), args[1].toInt32()))
        return Plugin::setError(ctx, "IO::seek Error: Internal error");

    return true; 
}

bool IO::size(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    IO *that = Plugin::getSelfObject<IO>(ctx, vp, &args, &IOPlugin::io_class);
    if(!that)
        return Plugin::setError(ctx, "IO::size Error: Invalid IO Object");
    
    args.rval().set(JS::RootedValue(ctx, JS_NumberValue(that->size())));
    return true;
}

bool IO::setPosition(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    IO *that = Plugin::getSelfObject<IO>(ctx, vp, &args, &IOPlugin::io_class);
    if(!that)
        return Plugin::setError(ctx, "IO::setPosition Error: Invalid IO Object");

    if(!that->seekable())
        return Plugin::setError(ctx, "IO::setPosition Error: IO Object is not seekable");

    if(args.length()<1)
        return Plugin::setError(ctx, "IO::setPosition Error: requires two arguments");

    if(!args[0].isNumber())
        return Plugin::setError(ctx, "IO::setPosition Error: argument 0 must be a number.");

    if(!that->seek(args[0].toNumber(), SEEK_SET))
        return Plugin::setError(ctx, "IO::setPosition Error: Internal error");

    args.rval().set(args[0]);
    return true; 
}

bool IO::getPosition(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    IO *that = Plugin::getSelfObject<IO>(ctx, vp, &args, &IOPlugin::io_class);
    if(!that)
        return Plugin::setError(ctx, "IO::getPosition Error: Invalid IO Object");
    
    args.rval().set(JS::RootedValue(ctx, JS_NumberValue(that->at())));
    return true;
}

bool IO::read(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    IO *that = Plugin::getSelfObject<IO>(ctx, vp, &args, &IOPlugin::io_class);
    if(!that)
        return Plugin::setError(ctx, "IO::read Error: Invalid IO Object");

    if(args.length()<1)
        return Plugin::setError(ctx, "IO::read Error: requires one argument");

    if(!args[0].isNumber())
        return Plugin::setError(ctx, "IO::read Error: argument 0 must be a number.");

    const unsigned length = args[0].toInt32();
    void *const buffer = JS_malloc(ctx, length);
    
    if(!buffer)
        return false;

    if(!that->read(buffer, length)){
        JS_free(ctx, buffer);
        return Plugin::setError(ctx, "IO::read Error: Internal error");
    }
    
    args.rval().set(JS::ObjectOrNullValue(JS_NewArrayBufferWithContents(ctx, length, buffer)));

    return true;

}

bool IO::write(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    IO *that = Plugin::getSelfObject<IO>(ctx, vp, &args, &IOPlugin::io_class);
    if(!that)
        return Plugin::setError(ctx, "IO::write Error: Invalid IO Object");

    if(!that->writable())
        return Plugin::setError(ctx, "IO::write Error: Object is not writable");

    if(args.length()<1)
        return Plugin::setError(ctx, "IO::write Error: requires one argument");
    
    if(args[0].isString()){
        const char *const str = JS_EncodeStringToUTF8(ctx, JS::RootedString(ctx, args[0].toString()));
        const unsigned length = JS_GetStringLength(args[0].toString());

        const bool err = that->write(str, strnlen(str, length*4));

        JS_free(ctx, (void *)str);

        return err || Plugin::setError(ctx, "IO::write Error: Internal error");
    }
/*
    if(CheckForSingleArg(ctx, args, Turbo::TypedArray, __func__, false)){
        JS::RootedObject buffer_root(ctx, args[0].toObjectOrNull());
        js::GetArrayBufferViewLengthAndData(buffer_root, &l, &data);
    }
*/
    else
        return Plugin::setError(ctx, "IO::write Error: Invalid type for argument 0");
    
    return true;
}

void IO::IOFinalizer(JSFreeOp *fop, JSObject *obj){
    delete static_cast<IO *>(JS_GetPrivate(obj));
    // Ensure that the pointer is lost...
    JS_SetPrivate(obj, nullptr);
}

}
