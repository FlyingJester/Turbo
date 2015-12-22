#include "plugin.hpp"
#include "io.hpp"
#include "std_c_file.hpp"
#include <array>
#include <cstdio>
#include <cassert>

namespace Ultra{

class FilePlugin : public Plugin {

    std::array<std::pair<const char *, IO *>, 3> file_variables;
    static const std::array<std::pair<const char *, int>, 3> seek_variables;

public:
    
    static JSClass file_class;
    JS::Heap<JSObject *> file_prototype;

    FilePlugin()
      : Plugin("file"){

    }

    void init(JSContext *ctx) override{
        file_prototype = JS_NewPlainObject(ctx);
        file_variables[0] = {"stdin", StdCFile::getStdin() };
        file_variables[1] = {"stdout",StdCFile::getStdout()};
        file_variables[2] = {"stderr",StdCFile::getStderr()};

        Plugin::init(ctx);    
    }

    int numFunctions() override {
        return 0;
    }

    JSNative functionCallback(int i) override {
        return nullptr;
    }


    const char *functionName(int i) override {
        return nullptr;
    }

    int numVariables() override {
        return file_variables.size() + seek_variables.size();
    }

    void variableValue(int e, JS::MutableHandleValue vp) override {
        if(e<0) return;
        unsigned i = e;
        JS::RootedObject prototype(context(), file_prototype), outval(context());

        if(i<file_variables.size()){
            wrapNativeObject(context(), file_variables[i].second, &file_class, prototype, &outval);
            vp.set(ObjectOrNullValue(outval));
        }
        else{
            i-=file_variables.size();
            if(i<seek_variables.size()){
                vp.set(JS_NumberValue(seek_variables[i].second));
            }
        }
    }

    const char *variableName(int e) override {
        if(e<0) return nullptr;
        unsigned i = e;

        if(i<file_variables.size()){
            return file_variables[i].first;
        }
        else{
            i-=file_variables.size();
            if(i<seek_variables.size()){
                return seek_variables[i].first;
            }
            else
                return nullptr;
        }
    }

    static bool FileConstructor(JSContext *ctx, unsigned argc, JS::Value *vp){
        
        
        return true;
    }
    static void FileFinalizer(JSFreeOp *fop, JSObject *obj){
        delete static_cast<IO *>(JS_GetPrivate(obj));
    }
};

const std::array<std::pair<const char *, int>, 3> FilePlugin::seek_variables = {{
    { "seek_cur", SEEK_CUR },
    { "seek_set", SEEK_SET },
    { "seek_end", SEEK_END }
}};

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
    FilePlugin::FileFinalizer,
    nullptr,
    nullptr,
    FilePlugin::FileConstructor,
    nullptr,
    nullptr
};

extern "C" Plugin *UltraPlugin(){
    assert(FilePlugin::file_class.finalize  == FilePlugin::FileFinalizer);
    assert(FilePlugin::file_class.construct == FilePlugin::FileConstructor);
    return new FilePlugin();
}

} // namespace Ultra
