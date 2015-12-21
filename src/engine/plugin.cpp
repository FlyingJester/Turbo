#include "plugin.hpp"

namespace Ultra{

Plugin::Plugin()
  : name_(nullptr)
  , m_ctx(nullptr){

}

Plugin::Plugin(const char *a_name)
  : name_(a_name)
  , m_ctx(nullptr){

}

const char *Plugin::name(){
    return name_;
}

    // This is guaranteed to be called before any other members.
void Plugin::init(JSContext *ctx){
    m_ctx = ctx;
}

} // namespace Ultra
