#include "script.hpp"
#include <fstream>
#include <sstream>

namespace Ultra{

bool RunScript(JSContext *ctx, const std::string &directory, const std::string &script_name){

    std::stringstream buffer;

    std::ifstream configfile(directory + '/' + script_name);
    if(!configfile){
        fprintf(stderr, "[UltraSphere]Could not open script %s\n", script_name.c_str());
        return false;
    }
    buffer << configfile.rdbuf();

    JS::OwningCompileOptions options(ctx);

    options.setFileAndLine(ctx, script_name.c_str(), 0);
    options.setIntroductionScript(nullptr).setVersion(JSVERSION_LATEST).setUTF8(true);

    options.asmJSOption = true;
    options.strictOption = true;

    JS::RootedScript script(ctx);
    JS::RootedValue rval(ctx);

    JS::RootedObject global(ctx);
    global.set(JS::CurrentGlobalOrNull(ctx));

    if(!JS::Compile(ctx, global, options, buffer.str().c_str(), buffer.str().length(), &script)){
        fprintf(stderr, "[UltraSphere]Could not compile script %s\n", script_name.c_str());
        return false;
    }
    
    JS_MaybeGC(ctx);
    
    if(!JS_ExecuteScript(ctx, global, script, &rval)){
        fprintf(stderr, "[UltraSphere]Could not execute script %s\n", script_name.c_str());
        return false;
    }
    
    return true;
}

} // namespace Ultra
