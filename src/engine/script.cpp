#include "script.hpp"
#include <fstream>
#include <sstream>

namespace Turbo{

bool RunScript(JSContext *ctx, const std::string &directory, const std::string &script_name){

    std::stringstream buffer;

    std::ifstream configfile(directory + '/' + script_name);
    if(!configfile){
        fprintf(stderr, "[Turbo]Could not open script %s\n", script_name.c_str());
        return false;
    }
    buffer << configfile.rdbuf();

    JS::OwningCompileOptions options(ctx);

    options.setFileAndLine(ctx, script_name.c_str(), 0);
    options.setIntroductionScript(nullptr).setVersion(JSVERSION_LATEST).setUTF8(true);

    options.asmJSOption = JS::AsmJSOption::Enabled;
    options.strictOption = true;

    JS::RootedScript script(ctx);
    JS::RootedValue rval(ctx);

    if(!JS::Compile(ctx, options, buffer.str().c_str(), buffer.str().length(), &script)){
        fprintf(stderr, "[Turbo]Could not compile script %s\n", script_name.c_str());
        return false;
    }
    
    JS_MaybeGC(ctx);
    
    if(!JS_ExecuteScript(ctx, script, &rval)){
        fprintf(stderr, "[Turbo]Could not execute script %s\n", script_name.c_str());
        return false;
    }
    
    return true;
}

} // namespace Turbo
