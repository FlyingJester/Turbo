#include "sapphire.hpp"
#include "window.hpp"

namespace Sapphire {

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
    Window::Finalizer, // finalizer
    nullptr,
    nullptr,
    Window::Constructor, // constructor
    nullptr,
    {nullptr}
};

} // namespace Sapphire
