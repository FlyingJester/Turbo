#pragma once
#include "plugin.hpp"
#include "io.hpp"
#include <array>

namespace Ultra{


class IOPlugin : public Plugin {

    std::array<std::pair<const char *, IO *>, 3> io_variables;
    static const std::array<std::pair<const char *, JSNative>, 1> functions;

    static const std::array<std::pair<const char *, int>, 3> seek_variables;
    static const std::array<std::pair<const char *, int>, 3> rw_variables;
    static const std::array<JSFunctionSpec, 5> io_methods;
    static const std::array<JSPropertySpec, 3> io_properties;

public:
    static const int readable = 1, writable = 2, readandwrite = readable|writable;

    // Classes and prototypes are held by the plugin object to
    // ensure their lifetimes match the context.
    static JSClass io_class, c_file_class;
    JS::Heap<JSObject *> io_prototype, c_file_prototype;

    IOPlugin() : Plugin("io"){}

    void init(JSContext *ctx) override;

    int numFunctions() override;
    JSNative functionCallback(int i);
    const char *functionName(int i);
    int numVariables() override;
    void variableValue(int e, JS::MutableHandleValue vp) override;
    const char *variableName(int e) override;

};

} // namespace Ultra
