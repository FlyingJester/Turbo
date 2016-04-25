#pragma once
#include "plugin.hpp"
#include <array>

namespace Sapphire {

class SapphirePlugin : public Turbo::Plugin {

    static const std::array<JSFunctionSpec, 3> window_methods;
    static const std::array<JSPropertySpec, 3> window_properties;

public:

    static JSClass window_class;

    JS::Heap<JSObject *> window_prototype;

    SapphirePlugin() : Plugin("sapphire"){}

    void init(JSContext *ctx) override;

    int numFunctions() override;
    JSNative functionCallback(int i) override;
    const char *functionName(int i) override;
    int numVariables() override;
    void variableValue(int e, JS::MutableHandleValue vp) override;
    const char *variableName(int e) override;

};

} // namespace Sapphire
