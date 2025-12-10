//
// Created by Rob Hendriks on 09/12/2025.
//

#pragma once

#include "value.h"
#include "runtime.h"

#include <string>
#include <string_view>

namespace qjsx {
    class Context final {
        friend class Value;

    public:
        using ModuleMap = std::unordered_map<std::string, JSModuleDef *>;

        Context(const Runtime &rt, void *userData);

        Context(const Context &) = delete;

        Context(Context &&) = delete;

        Context &operator=(const Context &) = delete;

        Context &operator=(Context &&) = delete;

        [[nodiscard]] Value eval(std::string_view source, std::string_view filename) const;

        [[nodiscard]] Value evalModule(std::string_view moduleName) const;

        [[nodiscard]] JSContext *ptr() const;

        explicit operator JSContext *() const;

        Value getModule(std::string_view moduleName) const;

        ModuleMap &getModules();

        const ModuleMap &getModules() const;

        template<typename T>
        T *getUserData() const {
            return static_cast<T *>(m_userData);
        }

        ~Context();

    private:
        ModuleMap m_modules;
        JSContext *m_context;
        void *m_userData;
    };
}
