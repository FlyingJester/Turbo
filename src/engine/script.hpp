#pragma once
#include <string>
#include <jsapi.h>

namespace Ultra{

bool RunScript(JSContext *ctx, const std::string &directory, const std::string &script_name);

} // namespace Ultra
