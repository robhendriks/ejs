//
// Created by Rob Hendriks on 10/12/2025.
//

#include "plugin_manager.h"

#include <ranges>
#include <vector>
#include <spdlog/spdlog.h>

#include "app.h"

namespace ejs {
    static std::vector<PluginFile> discoverPluginFiles(const fs::path &path) {
        std::vector<PluginFile> pluginFiles;

        if (!fs::is_directory(path)) {
            return pluginFiles;
        }

        for (const auto &entry: fs::directory_iterator(path)) {
            if (!entry.is_directory()) {
                continue;
            }

            const auto pluginFile = PluginFile::fromPath(entry.path());

            if (pluginFile.isValid()) {
                pluginFiles.push_back(pluginFile);
            }
        }

        return pluginFiles;
    }

    PluginManager::PluginManager(App &app)
        : m_app(app),
          m_pluginsPath(app.getOptions().workingDirectory / "plugins") {
        m_runtime.addModulePath("plugins/*", m_pluginsPath.string() + "/*/main.js");
    }

    void PluginManager::loadPlugins() {
        const auto pluginFiles = discoverPluginFiles(m_pluginsPath);

        for (const auto &pluginFile: pluginFiles) {
            const auto pluginInfo = PluginInfo::fromPath(pluginFile.manifestFile);

            const auto ns = "plugins/" + pluginFile.rootDirectory.filename().string();
            auto plugin = new Plugin(this, pluginFile, pluginInfo, ns);

            const auto it = m_plugins.try_emplace(pluginInfo.id, plugin);

            if (it.second) {
                spdlog::info("Loaded plugin {} ({})", pluginInfo.name, pluginInfo.id);
            } else {
                spdlog::warn("Duplicate plugin identifier: ", pluginInfo.id);
            }
        }
    }

    void PluginManager::enablePlugins() {
        for (const auto &val: m_plugins | std::views::values) {
            val->enable();
        }
    }

    Runtime &PluginManager::getRuntime() {
        return m_runtime;
    }
}
