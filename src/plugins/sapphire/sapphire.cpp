#include "sapphire.hpp"

JSClass SapphirePlugin::window_class = {
    "File",
    JSCLASS_HAS_PRIVATE,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    WindowFinalizer, // finalizer
    nullptr,
    nullptr,
    WindowConstructor, // constructor
    nullptr,
    {nullptr}
};
