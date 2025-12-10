//
// Created by Rob Hendriks on 10/12/2025.
//

#pragma once

#include <string>
#include <filesystem>
#include <qjsx/qjsx.h>

namespace ejs {
    using namespace qjsx;

    namespace fs = std::filesystem;

    class PluginManager;

    struct PluginFile final {
        fs::path rootDirectory;
        fs::path manifestFile;
        fs::path moduleFile;

        static PluginFile fromPath(const fs::path &path);

        [[nodiscard]] bool isValid() const;
    };

    struct PluginInfo final {
        std::string id;
        std::string name;

        static PluginInfo fromPath(const fs::path &path);
    };

    class Plugin final {
    public:
        Plugin(PluginManager *manager, PluginFile file, PluginInfo info, std::string ns);

        Plugin(const Plugin &) = delete;

        Plugin(Plugin &&) = delete;

        Plugin &operator=(const Plugin &) = delete;

        Plugin &operator=(Plugin &&) = delete;

        void enable();

        [[nodiscard]] PluginManager *getManager() const;

    private:
        bool prepareScripting();

        std::unique_ptr<Context> m_context;
        PluginManager *m_manager;
        const PluginFile m_file;
        const PluginInfo m_info;
        const std::string m_ns;
    };
}
