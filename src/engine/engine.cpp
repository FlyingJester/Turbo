#include "plugin.hpp"
#include "engine.hpp"
#include "plugloader.hpp"
#include "script.hpp"

#include <js/Initialization.h>

#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>

namespace Turbo{

Engine::Engine()
  : Plugin("Turbo"){
    memset(&global_, 0, sizeof(JSClass));
    global_.name = "global";
    global_.flags = JSCLASS_GLOBAL_FLAGS;
    global_.trace = JS_GlobalObjectTraceHook;
    
}

Engine::~Engine(){

}

int Engine::numFunctions(){
    return 0;
}

JSNative Engine::functionCallback(int i){
    return nullptr;
}
const char *Engine::functionName(int i){
    return nullptr;
}

int Engine::numVariables(){
    return 0;
}

void Engine::variableValue(int i, JS::MutableHandleValue vp){

}

const char *Engine::variableName(int i){
    return nullptr;
}

// Use SpiderMonkey's builtin JSON decoder to get our configuration info.
static bool LoadConfig(JSContext *ctx, const std::string &filename, JS::MutableHandleObject to){

    std::stringstream buffer;

    std::ifstream configfile(filename);
    if(!configfile){
        fprintf(stderr, "[Turbo]Could not open configuration file %s\n", filename.c_str());
        return false;
    }
    buffer << configfile.rdbuf();

    JS::RootedValue to_val(ctx);
    JS::RootedString config(ctx);
    config.set(JS_NewStringCopyN(ctx, buffer.str().c_str(), buffer.str().size()));

    if(!(JS_ParseJSON(ctx, config, &to_val) && JS_ValueToObject(ctx, to_val, to))){
        fprintf(stderr, "[Turbo]Could not parse JSON file %s\n", filename.c_str());
        return false;
    }
    return true;
}

static bool LoadPluginsFromConfig(JSContext *ctx, JS::HandleValue plugin_list_val, std::vector<PluginHandle> &plugins){

    JS::RootedObject plugin_list(ctx);

    uint32_t length;
    // Something is dreadfully wrong if this fails when JS_IsArrayObject succeeded.
    if(!(JS_ValueToObject(ctx, plugin_list_val, &plugin_list) && JS_GetArrayLength(ctx, plugin_list, &length))){
        fprintf(stderr, "[Turbo]Internal error handling plugin array\n");
        return false;
    }
    else{
        JS::RootedValue plugin_val(ctx);
        for(uint32_t i = 0; i<length; i++){
            JS_GetElement(ctx, plugin_list, i, &plugin_val);
            if(const char *plugin_name = JS_EncodeString(ctx, plugin_val.toString())){
                
                PluginHandle plugin = LoadPlugin("plugins", plugin_name);
                
                if(plugin.plugin())
                    plugins.push_back(std::move(plugin));
                else
                    fprintf(stderr, "[Turbo]Could not load plugin %s\n", plugin_name);

                JS_free(ctx, (void *)plugin_name);
            }
            else{
                fprintf(stderr, "[Turbo]Invalid plugin %i\n", i);
                return false;
            }
        }
    }
    return true;
}

static bool InitPlugins(JSContext *ctx, std::vector<PluginHandle> &plugins){

    JS::RootedObject global(ctx);
    global.set(JS::CurrentGlobalOrNull(ctx));

    for(PluginHandle &plugin : plugins){
        plugin->init(ctx);

        const char *name = plugin->name();
        JS::RootedObject plugin_obj(ctx);

        // Detect conflicts.
        JS::RootedValue tmp(ctx);
        if(JS_GetProperty(ctx, global, name, &tmp) && tmp!=JS::UndefinedHandleValue){
            fprintf(stderr, "[Turbo]Plugin name '%s' is conflicting.\n", name);
            continue;
        }

        plugin->getObject(&plugin_obj);
        plugin_obj.set(plugin->getObject());

        tmp.set(JS::ObjectOrNullValue(plugin->getObject()));
        JS_SetProperty(ctx, global, name, tmp);

        for(int i = 0; i<plugin->numFunctions(); i++){
            JS_DefineFunction(ctx, plugin_obj,
                plugin->functionName(i),
                plugin->functionCallback(i), 
                0, 0);
        }

        for(int i = 0; i<plugin->numVariables(); i++){
            JS::RootedValue val(ctx);
            plugin->variableValue(i, &val);
            JS_SetProperty(ctx, plugin_obj, plugin->variableName(i), val);
        }
#ifndef NDEBUG
        printf("[Turbo]Loaded plugin %s\n", name);
#endif
    }
    return true;

}

static bool LoadScriptsFromConfig(JSContext *ctx, JS::HandleValue script_list_val){

    JS::RootedObject script_list(ctx);

    uint32_t length;
    // Something is dreadfully wrong if this fails when JS_IsArrayObject succeeded.
    if(!(JS_ValueToObject(ctx, script_list_val, &script_list) && JS_GetArrayLength(ctx, script_list, &length))){
        fprintf(stderr, "[Turbo]Internal error handling plugin array\n");
        return false;
    }
    else{
#ifndef NDEBUG
        printf("[Turbo]Loading %i scripts.\n", (int)length);
#endif
        JS::RootedValue script_name_val(ctx);
        for(uint32_t i = 0; i<length; i++){
            JS_GetElement(ctx, script_list, i, &script_name_val);
            if(const char *script_name = JS_EncodeString(ctx, script_name_val.toString())){
                const bool err = RunScript(ctx, "scripts", script_name);
                JS_free(ctx, (void *)script_name);
                if(!err)
                    return false;
            }
        }
    }
    return true;
}

// Parse and operate on a SpiderMonkey object representing our configuration.
static bool RunConfig(JSContext *ctx, JS::RootedObject &config, std::vector<PluginHandle> &plugins){
    JS::RootedValue plugin_list_val(ctx), script_list_val(ctx);
    bool is_array;
    if(!(JS_GetProperty(ctx, config, "plugins", &plugin_list_val) && JS_IsArrayObject(ctx, plugin_list_val, &is_array) && is_array)){
        fprintf(stderr, "[Turbo]Config lacks 'plugins' attribute as an array\n"); 
        return false;
    }

    if(!(JS_GetProperty(ctx, config, "scripts", &script_list_val) && JS_IsArrayObject(ctx, script_list_val, &is_array) && is_array)){
        fprintf(stderr, "[Turbo]Config lacks 'scripts' attribute as an array\n"); 
        return false;
    }

    if(!LoadPluginsFromConfig(ctx, plugin_list_val, plugins))
        return false;

    if(!InitPlugins(ctx, plugins))
        return false;

    if(!LoadScriptsFromConfig(ctx, script_list_val))
        return false;

    return true;
}

static void ReportError(JSContext *ctx, const char *msg, JSErrorReport *report) {
    
    const char * const filename = (report->filename)?report->filename:"";
    
    const char * error_type = "Generic Error";
    if(JSREPORT_IS_WARNING(report->flags))
        error_type="Warning";
    else if(JSREPORT_IS_EXCEPTION(report->flags))
        error_type="Exception";
    else if(JSREPORT_IS_STRICT_MODE_ERROR(report->flags))
        error_type="StrictError";
    
    fprintf(stderr, "[Turbo] %s in file %s line %i:\n\t%s\n",
        error_type, filename, report->lineno+1, msg);

}

class TurboAutoJS{
public:
    TurboAutoJS(){
        if(!JS_Init()){
            fputs("Error initializing SpiderMonkey.", stderr);
            exit(EXIT_FAILURE);
        }
    }
    ~TurboAutoJS(){ JS_ShutDown(); }
};

extern "C" int main(int argc, char *argv[]){
    TurboAutoJS js;
    
    std::unique_ptr<JSRuntime, void(*)(JSRuntime *)> runtime_ptr(JS_NewRuntime(8L * 1024L * 1024L), JS_DestroyRuntime);
    JSRuntime *const runtime = runtime_ptr.get();
    
    //JS::RuntimeOptionsRef(runtime).setIon(true).setBaseline(true).setAsmJS(true).setExtraWarnings(true);
    
    std::unique_ptr<JSContext, void(*)(JSContext *)> context_ptr(JS_NewContext(runtime, 8192), JS_DestroyContext);
    if(JSContext *const ctx = context_ptr.get()){ // Scope for autorequest

        Engine engine;

        JS_SetErrorReporter(runtime, ReportError);

        JSAutoRequest request(ctx);
        JS::RootedObject global(ctx, JS_NewGlobalObject(ctx, &engine.global(), nullptr, JS::FireOnNewGlobalHook));
        
        if(!global){
            
            return false;
        }
        
        JSAutoCompartment compartment(ctx, global);
        
        if(!JS_InitStandardClasses(ctx, global)){
            fprintf(stderr, "[Turbo]Could not initialize JS standard classes\n");
            return EXIT_FAILURE;
        }
        std::vector<PluginHandle> plugins;

        JS::RootedObject config(ctx);
        if(!LoadConfig(ctx, "Turbo.json", &config))
            return EXIT_FAILURE;
        
        // Our config is immutable now.
        JS_DeepFreezeObject(ctx, config);
        
        RunConfig(ctx, config, plugins);
    
        return EXIT_SUCCESS;
    }
    else
        return EXIT_FAILURE;
}

} // namespace Turbo
