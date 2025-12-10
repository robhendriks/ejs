//
// Created by Rob Hendriks on 10/12/2025.
//

#include "plugin.h"
#include "plugin_yaml.h"

#include <utility>
#include <spdlog/spdlog.h>

#include "plugin_manager.h"

namespace ejs {
    PluginFile PluginFile::fromPath(const fs::path &path) {
        return {
            .rootDirectory = path,
            .manifestFile = path / "plugin.yaml",
            .moduleFile = path / "main.js",
        };
    }

    bool PluginFile::isValid() const {
        if (!fs::is_regular_file(manifestFile)) {
            return false;
        }

        if (!fs::is_regular_file(moduleFile)) {
            return false;
        }

        return true;
    }

    PluginInfo PluginInfo::fromPath(const fs::path &path) {
        const YAML::Node doc = YAML::LoadFile(path.string());
        return doc.as<PluginInfo>();
    }

    Plugin::Plugin(PluginManager *manager, PluginFile file, PluginInfo info, std::string ns)
        : m_context(nullptr),
          m_manager(manager),
          m_file(std::move(file)),
          m_info(std::move(info)),
          m_ns(std::move(ns)) {
    }

    void Plugin::enable() {
        if (!prepareScripting()) {
            return;
        }

        spdlog::debug("Running main function");

        const Value mod = m_context->getModule(m_ns);
        const Value main = mod["main"];

        const Value mainVal = main();

        if (mainVal.isException()) {
            mainVal.printException();
            return;
        }

        // TODO
    }

    PluginManager *Plugin::getManager() const {
        return m_manager;
    }

    bool Plugin::prepareScripting() {
        if (m_context) {
            return true;
        }

        // Create JavaScript context
        m_context = std::make_unique<Context>(m_manager->getRuntime(), this);

        // Evaluate JavaScript module
        spdlog::debug("Compiling module {}", m_ns);

        const auto evalResult = m_context->evalModule(m_ns);
        if (evalResult.isException()) {
            evalResult.printException();
            return false;
        }

        return true;
    }
}
