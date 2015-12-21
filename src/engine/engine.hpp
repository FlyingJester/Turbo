#pragma once
#include "plugin.hpp"
#include <memory>
#include <jsapi.h>

namespace Ultra {

class Engine : public Plugin{
    JSClass global_;
public:
    Engine();
    virtual ~Engine();

    inline JSClass &global() { return global_; }
    inline const JSClass &global() const { return global_; }

    int numFunctions() override;
    JSNative functionCallback(int i) override;
    const char *functionName(int i) override;

    int numVariables() override;
    void variableValue(int i, jsval *vp) override;
    const char *variableName(int i) override;

};


} // namespace Ultra
