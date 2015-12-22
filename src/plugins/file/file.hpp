#pragma once
#include "plugin.hpp"
#include "io.hpp"
#include <array>

namespace Ultra{


class FilePlugin : public Plugin {

    std::array<std::pair<const char *, IO *>, 3> io_variables;
    static const std::array<std::pair<const char *, int>, 3> seek_variables;
    static const std::array<JSFunctionSpec, 5> io_methods;

public:
    
    static JSClass io_class;
    JS::Heap<JSObject *> io_prototype;

    FilePlugin() : Plugin("file"){}

    void init(JSContext *ctx) override;

    int numFunctions() override;
    JSNative functionCallback(int i);
    const char *functionName(int i);
    int numVariables() override;
    void variableValue(int e, JS::MutableHandleValue vp) override;
    const char *variableName(int e) override;

    static bool IOConstructor(JSContext *ctx, unsigned argc, JS::Value *vp);
    static void IOFinalizer(JSFreeOp *fop, JSObject *obj);
};

} // namespace Ultra
