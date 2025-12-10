//
// Created by Rob Hendriks on 09/12/2025.
//

#pragma once

#include <string>
#include <optional>
#include <quickjs.h>
#include <unordered_map>
#include <filesystem>

namespace qjsx {
    namespace fs = std::filesystem;

    class Runtime final {
        friend class Context;

    public:
        using ModulePathMap = std::unordered_map<std::string, std::string>;

        Runtime();

        Runtime(const Runtime &) = delete;

        Runtime(Runtime &&) = delete;

        Runtime &operator=(const Runtime &) = delete;

        Runtime &operator=(Runtime &&) = delete;

        [[nodiscard]] JSRuntime *ptr() const;

        explicit operator JSRuntime *() const;

        void addModulePath(const std::string &prefix, const std::string &path);

        [[nodiscard]] std::optional<fs::path> resolveModulePath(std::string_view moduleName) const;

        ~Runtime();

    private:
        ModulePathMap m_paths;
        JSRuntime *m_runtime;
    };
}
