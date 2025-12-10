//
// Created by Rob Hendriks on 10/12/2025.
//

#pragma once

#include "plugin_manager.h"

#include <filesystem>

namespace ejs {
    namespace fs = std::filesystem;

    struct AppOptions {
        fs::path workingDirectory;

        static AppOptions getDefault();
    };

    class App {
    public:
        explicit App(AppOptions options);

        void init();

        [[nodiscard]] const AppOptions &getOptions() const;

    private:
        const AppOptions m_options;
        PluginManager m_pluginManager;
    };
}
