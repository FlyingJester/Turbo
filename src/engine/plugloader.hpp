#pragma once
#include "plugin.hpp"
#include <string>
#include <jsapi.h>

namespace Ultra {

class PluginHandle{
    void *handle_;
    Plugin *plugin_;
public:

    PluginHandle(const PluginHandle & p) = delete;

    PluginHandle( PluginHandle && p)
      : handle_(p.handle_)
      , plugin_(p.plugin_){
        p.handle_ = nullptr;
        p.plugin_ = nullptr;
    }

    PluginHandle(void *handle)
      : handle_(handle){
        plugin_ = nullptr;
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

} // namespace Ultra
