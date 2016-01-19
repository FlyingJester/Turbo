#pragma once
#include <string>
#include <jsapi.h>

namespace Turbo{

bool RunScript(JSContext *ctx, const std::string &directory, const std::string &script_name);

} // namespace Turbo
