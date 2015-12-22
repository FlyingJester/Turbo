#include "plugin.hpp"
#include <array>
#include <cstdio>

namespace Ultra{

class FilePlugin : public Plugin {

    static const std::array<std::pair<const char *, FILE *>, 3> file_variables;
    static const std::array<std::pair<const char *, int>, 3> seek_variables;

public:
    
    FilePlugin()
      : Plugin("file"){

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

        if(i<file_variables.size()){

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

};

const std::array<std::pair<const char *, FILE *>, 3> FilePlugin::file_variables = {{
    { "stdout", stdout },
    { "stdin",  stdin  },
    { "stderr", stderr }
}};

const std::array<std::pair<const char *, int>, 3> FilePlugin::seek_variables = {{
    { "current",   SEEK_CUR },
    { "beginning", SEEK_SET },
    { "ending",    SEEK_END }
}};

extern "C" Plugin *UltraPlugin(){
    return new FilePlugin();
}

} // namespace Ultra
