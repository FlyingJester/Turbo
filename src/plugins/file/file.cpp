#include "file.hpp"

namespace Turbo {

// File callbacks.
static bool FileSeekable(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    FILE *that = Plugin::getSelfObject<FILE>(ctx, vp, &args, &FilePlugin::file_class);
    args.rval().set(JS::BooleanValue( that!=stdin && that!=stdout && that!=stderr && ftell(that) != -1L ));
    return true;
}

static bool FileSeek(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    FILE *that = Plugin::getSelfObject<FILE>(ctx, vp, &args, &FilePlugin::file_class);
    if(!that)
        return Plugin::setError(ctx, "FileSeek Error: Invalid File Object");

    if(args.length()<2)
        return Plugin::setError(ctx, "FileSeek Error: requires two arguments");

    if(!args[0].isNumber())
        return Plugin::setError(ctx, "FileSeek Error: argument 0 must be a number.");

    if(!args[1].isInt32() || args[1].toInt32() != SEEK_CUR || args[1].toInt32() != SEEK_END || args[1].toInt32() != SEEK_SET )
        return Plugin::setError(ctx, "FileSeek Error: arument 1 is an invalid origin, must be seek_cur, seek_set, or seek_end");

    if(fseek(that, args[0].toNumber(), args[1].toInt32())!=0)
        return Plugin::setError(ctx, "FileSeek Error: Could not seek.");

    return true; 
}

static bool FileRead(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    FILE *that = Plugin::getSelfObject<FILE>(ctx, vp, &args, &FilePlugin::file_class);
    if(!that)
        return Plugin::setError(ctx, "FileRead Error: Invalid File Object");

    if(args.length()<1)
        return Plugin::setError(ctx, "FileRead Error: requires one argument");

    if(!args[0].isNumber())
        return Plugin::setError(ctx, "FileRead Error: argument 0 must be a number.");

    const unsigned length = args[0].toInt32();
    void *const buffer = JS_malloc(ctx, length);
    
    if(!buffer)
        return false;

    if(fread(buffer, 1, length, that)!=length){
        JS_free(ctx, buffer);
        return Plugin::setError(ctx, "FileRead Error: Read error");
    }

    args.rval().set(JS::ObjectOrNullValue(JS_NewArrayBufferWithContents(ctx, length, buffer)));

    return true;

}

static bool FileWrite(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    FILE *that = Plugin::getSelfObject<FILE>(ctx, vp, &args, &FilePlugin::file_class);
    if(!that)
        return Plugin::setError(ctx, "FileWrite Error: Invalid IO Object");

    if(args.length()<1)
        return Plugin::setError(ctx, "FileWrite Error: requires one argument");
    
    if(args[0].isString()){
        const char *const str = JS_EncodeStringToUTF8(ctx, JS::RootedString(ctx, args[0].toString()));
        const unsigned length = JS_GetStringLength(args[0].toString());

        const bool err = fwrite(str, 1, strnlen(str, length*4), that)!=0;

        JS_free(ctx, (void *)str);

        return err || Plugin::setError(ctx, "FileWrite Error: Internal error");
    }
/*
    if(CheckForSingleArg(ctx, args, Turbo::TypedArray, __func__, false)){
        JS::RootedObject buffer_root(ctx, args[0].toObjectOrNull());
        js::GetArrayBufferViewLengthAndData(buffer_root, &l, &data);
    }
*/
    else
        return Plugin::setError(ctx, "FileWrite Error: Invalid type for argument 0");
    
    return true;
}

static bool FileSetPosition(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    FILE *that = Plugin::getSelfObject<FILE>(ctx, vp, &args, &FilePlugin::file_class);
    if(!that)
        return Plugin::setError(ctx, "FileSetPosition Error: Invalid IO Object");

    if(args.length()<1)
        return Plugin::setError(ctx, "FileSetPosition Error: requires two arguments");

    if(!args[0].isNumber())
        return Plugin::setError(ctx, "FileSetPosition Error: argument 0 must be a number.");

    if(fseek(that, args[0].toNumber(), SEEK_SET))
        return Plugin::setError(ctx, "FileSetPosition Error: Internal error");

    args.rval().set(args[0]);
    return true; 
}

static bool FileGetPosition(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    FILE *that = Plugin::getSelfObject<FILE>(ctx, vp, &args, &FilePlugin::file_class);
    if(!that)
        return Plugin::setError(ctx, "FileGetPosition Error: Invalid File Object");
    
    args.rval().set(JS::RootedValue(ctx, JS_NumberValue(ftell(that))));
    return true;
}

static bool FileSize(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    FILE *that = Plugin::getSelfObject<FILE>(ctx, vp, &args, &FilePlugin::file_class);
    if(!that)
        return Plugin::setError(ctx, "FileSize Error: Invalid File Object");
    
    fpos_t pos;
    fgetpos(that, &pos);
    
    fseek(that, 0, SEEK_END);
    const long len = ftell(that);
    
    fsetpos(that, &pos);

    args.rval().set(JS::RootedValue(ctx, JS_NumberValue(len)));
    return true;
}

static bool FileConstructor(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    
    if(args.length()<1)
        return Plugin::setError(ctx, "FileConstructor Error: requires one argument");
    
    if(args[0].isString()){
        const char *const str = JS_EncodeStringToUTF8(ctx, JS::RootedString(ctx, args[0].toString()));
        const char *mode = "rb";
        if(args.length()>1){
            if(!args[1].isInt32())
                return Plugin::setError(ctx, "FileConstructor Error: when present, argument 1 must be a read/write constant");
            switch(args[1].toInt32()){
                case FilePlugin::readable:
                    mode = "rb";
                    break;
                case FilePlugin::writable:
                    mode = "ab";
                    break;
                case FilePlugin::readandwrite:
                    mode = "ab";
                    break;
                default:
                return Plugin::setError(ctx, "FileConstructor Error: invalid read/write mode");
            }
        }

        FILE *const file = fopen(str, mode);

        if(!file){
            Plugin::setError(ctx, (std::string("FileConstructor Error: cannot open file ") + str).c_str());
            fputs("Opened file ", stdout); puts(str);
        }

        JS_free(ctx, (void *)str);

        if(!file)
            return false;

        JS::RootedObject that(ctx, JS_NewObjectForConstructor(ctx, &FilePlugin::file_class, args));
        JS_SetPrivate(that, file);
        args.rval().set(JS::ObjectOrNullValue(that));
    }
    else
        return Plugin::setError(ctx, "FileConstructor Error: Invalid type for argument 0");

    return true;
}

static void FileFinalizer(JSFreeOp *fop, JSObject *obj){
    fclose(static_cast<FILE *>(JS_GetPrivate(obj)));
    // Ensure that the pointer is lost...
    JS_SetPrivate(obj, nullptr);
}

JSClass FilePlugin::file_class = {
    "File",
    JSCLASS_HAS_PRIVATE,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    FileFinalizer, // finalizer
    nullptr,
    nullptr,
    FileConstructor, // constructor
    nullptr,
    {nullptr}
};

void FilePlugin::init(JSContext *ctx){
    
    Plugin::init(ctx);

    JS::RootedObject l_plugin_obj(ctx, getObject()),
        object_proto(ctx, JS_GetObjectPrototype(ctx, l_plugin_obj));
    file_prototype = JS_InitClass(ctx, l_plugin_obj, object_proto,
        &file_class, FileConstructor, 0, file_properties.data(), file_methods.data(), nullptr, nullptr);

}

int FilePlugin::numFunctions(){
    return 0;
}

JSNative FilePlugin::functionCallback(int i){
    return nullptr;
}

const char *FilePlugin::functionName(int i){
    return nullptr;
}

int FilePlugin::numVariables(){
    return 3 + seek_variables.size() + rw_variables.size();
}

void FilePlugin::variableValue(int e, JS::MutableHandleValue vp){
    if(e<0) return;
    unsigned i = e;
    JS::RootedObject prototype(context(), file_prototype), outval(context());

    if(i<3){
        FILE *file = nullptr;
        if(i==0) file = stdin;
        else if(i==1) file = stdout;
        else if(i==2) file = stderr;
        wrapNativeObject(context(), file, &file_class, prototype, &outval);
        vp.set(ObjectOrNullValue(outval));
        return;
    }

    i-=3;
    if(i<seek_variables.size()){
        vp.set(JS_NumberValue(seek_variables[i].second));
        return;
    }

    i-=seek_variables.size();
    if(i<rw_variables.size()){
        vp.set(JS_NumberValue(rw_variables[i].second));
        return;
    }
}

const char *FilePlugin::variableName(int e){

    if(e<0) return nullptr;
    unsigned i = e;

    if(i==0) return "stdin";
    if(i==1) return "stdout";
    if(i==2) return "stderr";

    i-=3;
    if(i<seek_variables.size())
        return seek_variables[i].first;

    i-=seek_variables.size();
    if(i<rw_variables.size())
        return rw_variables[i].first;

    return nullptr;
}

const std::array<std::pair<const char *, int>, 3> FilePlugin::seek_variables = {{
    { "seek_cur", SEEK_CUR },
    { "seek_set", SEEK_SET },
    { "seek_end", SEEK_END }
}};

const std::array<std::pair<const char *, int>, 3> FilePlugin::rw_variables = {{
    { "readable", FilePlugin::readable },
    { "writable", FilePlugin::writable },
    { "readandwrite", FilePlugin::readandwrite }
}};

const std::array<JSFunctionSpec, 5> FilePlugin::file_methods = {{
    JS_FN("read", FileRead, 1, 0),
    JS_FN("write", FileWrite, 1, 0),
    JS_FN("seek", FileSeek, 2, 0),
    JS_FN("seekable", FileSeekable, 0, 0),
    JS_FS_END
}};

const std::array<JSPropertySpec, 3> FilePlugin::file_properties = {{
    JS_PSG("size", FileSize, 0),
    JS_PSGS("position", FileGetPosition, FileSetPosition, 0),
    JS_PS_END
}};

extern "C" Plugin *TurboPlugin(){
    return new FilePlugin();
}

} // namespace Turbo
