//
// Created by Rob Hendriks on 10/12/2025.
//

#include "app.h"
#include "env.h"

#include <spdlog/spdlog.h>

namespace ejs {
    inline fs::path getWorkingDirectory() {
        const auto workingDirectory = env::getVariable("EJS_DIR");
        return workingDirectory.empty()
                   ? fs::current_path()
                   : fs::path{workingDirectory};
    }

    AppOptions AppOptions::getDefault() {
        return {
            .workingDirectory = getWorkingDirectory(),
        };
    }

    App::App(AppOptions options)
        : m_options(std::move(options)),
          m_pluginManager(*this) {
    }

    void App::init() {
#ifndef NDEBUG
        spdlog::set_level(spdlog::level::debug);
#endif

        m_pluginManager.loadPlugins();
        m_pluginManager.enablePlugins();
    }

    const AppOptions &App::getOptions() const {
        return m_options;
    }
}
