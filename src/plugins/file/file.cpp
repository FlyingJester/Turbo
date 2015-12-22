#include "file.hpp"
#include "io.hpp"
#include "std_c_file.hpp"
#include <cstdio>
#include <cassert>

namespace Ultra{

void FilePlugin::init(JSContext *ctx){
    
    Plugin::init(ctx);

    JS::RootedObject l_plugin_obj(ctx, getObject()),
        object_proto(ctx, JS_GetObjectPrototype(ctx, l_plugin_obj));
    io_prototype = JS_InitClass(ctx, l_plugin_obj, object_proto,
        &io_class, IOConstructor, 0, nullptr, io_methods.data(), nullptr, nullptr);
    io_variables[0] = {"stdin", StdCFile::getStdin() };
    io_variables[1] = {"stdout",StdCFile::getStdout()};
    io_variables[2] = {"stderr",StdCFile::getStderr()};
   
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
    return io_variables.size() + seek_variables.size();
}

void FilePlugin::variableValue(int e, JS::MutableHandleValue vp){
    if(e<0) return;
    unsigned i = e;
    JS::RootedObject prototype(context(), io_prototype), outval(context());

    if(i<io_variables.size()){
        wrapNativeObject(context(), io_variables[i].second, &io_class, prototype, &outval);
        vp.set(ObjectOrNullValue(outval));
    }
    else{
        i-=io_variables.size();
        if(i<seek_variables.size()){
            vp.set(JS_NumberValue(seek_variables[i].second));
        }
    }
}

const char *FilePlugin::variableName(int e){
    if(e<0) return nullptr;
    unsigned i = e;

    if(i<io_variables.size()){
        return io_variables[i].first;
    }
    else{
        i-=io_variables.size();
        if(i<seek_variables.size()){
            return seek_variables[i].first;
        }
        else
            return nullptr;
    }
}

bool FilePlugin::IOConstructor(JSContext *ctx, unsigned argc, JS::Value *vp){
    return true;
}

void FilePlugin::IOFinalizer(JSFreeOp *fop, JSObject *obj){
    delete static_cast<IO *>(JS_GetPrivate(obj));
}

const std::array<std::pair<const char *, int>, 3> FilePlugin::seek_variables = {{
    { "seek_cur", SEEK_CUR },
    { "seek_set", SEEK_SET },
    { "seek_end", SEEK_END }
}};

const std::array<JSFunctionSpec, 5> FilePlugin::io_methods = {{
    JS_FN("read", IO::read, 1, 0),
    JS_FN("write", IO::write, 1, 0),
    JS_FN("seek", IO::seek, 2, 0),
    JS_FN("seekable", IO::seekable, 0, 0),
    JS_FS_END
}};

JSClass FilePlugin::io_class = {
    "IO",
    JSCLASS_HAS_PRIVATE,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    FilePlugin::IOFinalizer,
    nullptr,
    nullptr,
    FilePlugin::IOConstructor,
    nullptr,
    nullptr
};

extern "C" Plugin *UltraPlugin(){
    return new FilePlugin();
}

} // namespace Ultra
