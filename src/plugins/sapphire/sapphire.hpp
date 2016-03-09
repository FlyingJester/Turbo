#pragma once
#include "plugin.hpp"
#include <array>

namespace Turbo{

/**
 * Objects:
 *    - Window
 *    - Image
 *    - Group
 *    - Shape
 *    - Shader
 *
 *    - Window
 *        + show()
 *        + hide()
 *        + flip()
 *        + clear()
 *        + w
 *        + h
 *        + shown
 *        + getKey()
 *        + areKeysLeft()
 *        + clearKeys()
 *        + getClicks()
 *        + areClicksLeft()
 *        + clearClicks()
 *        + getScroll()
 *        + getScrolls()
 *        + areScrollsLeft()
 *        + clearScrolls()
 *
 *    - Image(w, h, color) / Image(filename)? / Image(w, h, typedarray)
 *        + w
 *        + h
 *
 *    - Shader()?
 *        + attributes > ["attributeName1", "attributeName2", ...]
 *
 *    - Shape(Vertices, image)
 *        + image
 *        + x
 *        + y
 *        + rotX
 *        + rotY
 *        + angle
 *        + scale
 *
 *    - Group(shapes, shader)
 *        + shader
 *        + x
 *        + y
 *        + rotX
 *        + rotY
 *        + angle
 *        + scale
 *        + draw(Window)
 *
 */

class SapphirePlugin : public Plugin {

    static const std::array<JSFunctionSpec, 14> window_methods;
    static const std::array<JSPropertySpec, 2> window_properties;

    static const std::array<JSPropertySpec, 2> image_properties;

    static const std::array<JSPropertySpec, 1> shader_methods;

    static const std::array<JSPropertySpec, 7> shape_properties;

    static const std::array<JSFunctionSpec, 1> group_methods;
    static const std::array<JSPropertySpec, 7> group_properties;

public:
    static const int readable, writable, readandwrite;

    static JSClass window_class,
        image_class,
        shader_class,
        group_class,
        shape_class;

    JS::Heap<JSObject *> window_prototype,
        image_prototype,
        shader_prototype,
        group_prototype,
        shape_prototype;

    SapphirePlugin() : Plugin("sapphire"){}

    void init(JSContext *ctx) override;

    int numFunctions() override;
    JSNative functionCallback(int i) override;
    const char *functionName(int i) override;
    int numVariables() override;
    void variableValue(int e, JS::MutableHandleValue vp) override;
    const char *variableName(int e) override;

};

} // namespace Turbo
