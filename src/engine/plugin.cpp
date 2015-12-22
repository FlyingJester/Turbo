#include "plugin.hpp"

namespace Ultra{

Plugin::Plugin()
  : name_(nullptr)
  , ctx_(nullptr){

}

Plugin::Plugin(const char *a_name)
  : name_(a_name)
  , ctx_(nullptr){

}

const char *Plugin::name(){
    return name_;
}

    // This is guaranteed to be called before any other members.
void Plugin::init(JSContext *ctx){
    ctx_ = ctx;
}

} // namespace Ultra
