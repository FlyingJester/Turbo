#pragma once
#include "plugin.hpp"
#include <memory>
#include <jsapi.h>

namespace Turbo {

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
    void variableValue(int i, JS::MutableHandleValue vp) override;
    const char *variableName(int i) override;

};


} // namespace Turbo
