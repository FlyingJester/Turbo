#include "plugloader.hpp"
#ifdef _WIN32
    #include <windows.h>
#else
    #include <stdlib.h>
    #include <dlfcn.h>
#endif

namespace Turbo {

#ifdef _WIN32

    typedef HINSTANCE library_handle;

    library_handle LoadLibrary(const std::string &dir, const std::string &name){
        return ::LoadLibrary((dir + '\\' + name + ".dll").c_str());

    }

    void *LibraryFunction(library_handle that, const std::string &name){
        return GetProcAddress((HMODULE)that, name.c_str());
    }

    void CloseLibrary(void *that){
        FreeLibrary((HMODULE)that);
    }

#else

    typedef void * library_handle;

    #ifdef __APPLE__
    
        static const std::string library_suffix = ".dylib";

    #else

        static const std::string library_suffix = ".so";

    #endif

    library_handle LoadLibrary(const std::string &dir, const std::string &name){
        const std::string lib = dir + "/lib" + name + library_suffix;
        if(void *plug = dlopen(lib.c_str(), RTLD_LAZY|RTLD_LOCAL))
            return plug;
        else{
            fprintf(stderr, "[Turbo]Error loading shared library %s for plugin %s\n[TurboSphere]%s\n", lib.c_str(), name.c_str(), dlerror());
            return nullptr;
        }
    }

    void *LibraryFunction(library_handle that, const std::string &name){
        return dlsym(that, name.c_str());
    }

    void CloseLibrary(void *that){
        dlclose(that);
    }

#endif

PluginHandle::~PluginHandle(){
    if(handle_)
        CloseLibrary(handle_);
    if(plugin_)
        delete plugin_;
}

typedef Plugin *(*LoadPluginFunctionType)();

PluginHandle LoadPlugin(const std::string &directory, const std::string &plugin_name){
    
    if(library_handle handle = LoadLibrary(directory, plugin_name)){
        PluginHandle plugin(handle);
        LoadPluginFunctionType loader;

        if(!(loader = (LoadPluginFunctionType)LibraryFunction(handle, "TurboPlugin"))){
            fprintf(stderr, "[Turbo]No entry point for plugin %s\n", plugin_name.c_str());
            return PluginHandle(nullptr);
        }

        if(Plugin *p = loader()){
            plugin.plugin(p);
            return plugin;
        }
        else{
            fprintf(stderr, "[Turbo]Null plugin %s\n", plugin_name.c_str());
            return PluginHandle(nullptr);
        }
    }
    else{
        fprintf(stderr, "[Turbo]Could not open shared library for plugin %s\n", plugin_name.c_str());
        return PluginHandle(nullptr);
    }
}

} // namespace Turbo
