#pragma once
#include "plugin.hpp"
#include <array>

namespace Turbo{

class FilePlugin : public Plugin {

    static const std::array<std::pair<const char *, JSNative>, 1> functions;

    static const std::array<std::pair<const char *, int>, 3> seek_variables;
    static const std::array<std::pair<const char *, int>, 3> rw_variables;
    static const std::array<JSFunctionSpec, 5> file_methods;
    static const std::array<JSPropertySpec, 3> file_properties;

public:
    static const int readable, writable, readandwrite;

    static JSClass file_class;
    JS::Heap<JSObject *> file_prototype;

    FilePlugin() : Plugin("file"){}

    void init(JSContext *ctx) override;

    int numFunctions() override;
    JSNative functionCallback(int i) override;
    const char *functionName(int i) override;
    int numVariables() override;
    void variableValue(int e, JS::MutableHandleValue vp) override;
    const char *variableName(int e) override;

};

} // namespace Turbo
