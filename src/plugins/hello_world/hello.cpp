#include "plugin.hpp"

namespace Ultra{

class HelloPlugin : public Plugin {
public:

HelloPlugin()
  : Plugin("hello"){
    puts("Hello!");
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
    return 0;
}

void variableValue(int i, jsval *vp) override {

}

const char *variableName(int i) override {
    return nullptr;
}

};

extern "C" Plugin *UltraPlugin(){
    return new HelloPlugin();
}

} // namespace Ultra
