#pragma once
#include "plugin.hpp"
#include <string>
#include <jsapi.h>

namespace Turbo{

TURBO_EXPORT
bool RunScript(JSContext *ctx, const std::string &directory, const std::string &script_name);

} // namespace Turbo
