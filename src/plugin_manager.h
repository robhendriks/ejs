//
// Created by Rob Hendriks on 10/12/2025.
//

#pragma once

#include "plugin.h"

#include <unordered_map>
#include <qjsx/qjsx.h>

namespace ejs {
    using namespace qjsx;

    class App;

    class PluginManager final {
    public:
        using PluginMap = std::unordered_map<std::string, std::unique_ptr<Plugin> >;

        explicit PluginManager(App &app);

        PluginManager(const PluginManager &) = delete;

        PluginManager(PluginManager &&) = delete;

        PluginManager &operator=(const PluginManager &) = delete;

        PluginManager &operator=(PluginManager &&) = delete;

        void loadPlugins();

        void enablePlugins();

        [[nodiscard]] Runtime &getRuntime();

    private:
        App &m_app;
        fs::path m_pluginsPath;
        Runtime m_runtime;
        PluginMap m_plugins;
    };
}
