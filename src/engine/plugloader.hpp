#pragma once
#include "plugin.hpp"
#include <string>
#include <jsapi.h>

namespace Turbo {

class PluginHandle{
    void *handle_;
    Plugin *plugin_;
    std::string name_;
public:

    PluginHandle(const PluginHandle & p) = delete;

    PluginHandle( PluginHandle && p)
      : handle_(p.handle_)
      , plugin_(p.plugin_)
      , name_(p.name_){
        p.handle_ = nullptr;
        p.plugin_ = nullptr;
    }

    PluginHandle(void *handle, const std::string &name)
      : handle_(handle)
      , plugin_(nullptr)
      , name_(name){

    }
    ~PluginHandle();

    Plugin *plugin() { return plugin_; };
    const Plugin *plugin() const { return plugin_; };
    void plugin(Plugin *p) { plugin_ = p; };

    Plugin* operator->(){ return plugin(); }
    const Plugin* operator->() const { return plugin(); }
    Plugin& operator*(){ return *plugin(); }
    const Plugin& operator*() const { return *plugin(); }

};

// On return, the plugin will still need to be initialized.
PluginHandle LoadPlugin(const std::string &directory, const std::string &plugin_name);

} // namespace Turbo
