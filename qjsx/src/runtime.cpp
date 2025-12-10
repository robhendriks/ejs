//
// Created by Rob Hendriks on 09/12/2025.
//

#include "qjsx/runtime.h"

#include <iostream>

#include "qjsx/context.h"

#include <quickjs-libc.h>

namespace qjsx {
    static JSModuleDef *loadModuleFromFileSystem(JSContext *ctx, const std::string_view module_name,
                                                 const fs::path &path) {
        size_t buffer_size;
        uint8_t *buffer = js_load_file(ctx, &buffer_size, path.string().data());

        if (buffer == nullptr || buffer_size == 0) {
            return nullptr;
        }

        const JSValue compile_val = JS_Eval(
            ctx,
            reinterpret_cast<char *>(buffer), buffer_size,
            module_name.data(),
            JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY
        );

        js_free(ctx, buffer);
        if (JS_IsException(compile_val)) {
            js_std_dump_error(ctx);
            return nullptr;
        }

        if (js_module_set_import_meta(ctx, compile_val, true, false) < 0) {
            JS_FreeValue(ctx, compile_val);
            return nullptr;
        }

        const auto module_def = static_cast<JSModuleDef *>(JS_VALUE_GET_PTR(compile_val));
        JS_FreeValue(ctx, compile_val);

        return module_def;
    }

    static JSModuleDef *loadModuleWithFallback(JSContext *ctx, const char *moduleName, void *opaque) {
        const auto xRuntime = static_cast<Runtime *>(opaque);
        const auto xContext = static_cast<Context *>(JS_GetContextOpaque(ctx));

        const std::string_view prefix{moduleName};
        const auto path = xRuntime->resolveModulePath(prefix);
        if (path.has_value()) {
            // load module from disk
            JSModuleDef *def = loadModuleFromFileSystem(ctx, moduleName, path.value());

            // put module in registry
            xContext->getModules().emplace(prefix, def);

            return def;
        }

        return js_module_loader(ctx, moduleName, opaque);
    }

    static std::string buildModulePrefix(std::string prefix) {
        if (!prefix.ends_with("*")) {
            throw std::runtime_error("Module path should end with *");
        }

        prefix.replace(prefix.size() - 1, 1, "");

        return prefix;
    }

    Runtime::Runtime()
        : m_runtime(JS_NewRuntime()) {
        js_std_init_handlers(m_runtime);
        JS_SetRuntimeOpaque(m_runtime, this);
        JS_SetModuleLoaderFunc(m_runtime, nullptr, loadModuleWithFallback, this);
    }

    JSRuntime *Runtime::ptr() const {
        return m_runtime;
    }

    Runtime::operator JSRuntime *() const {
        return m_runtime;
    }

    void Runtime::addModulePath(const std::string &prefix, const std::string &path) {
        m_paths.emplace(
            buildModulePrefix(prefix),
            path
        );
    }

    std::optional<fs::path> Runtime::resolveModulePath(const std::string_view moduleName) const {
        for (const auto &path: m_paths) {
            if (moduleName.starts_with(path.first)) {
                const auto moduleSubPath = moduleName.substr(path.first.size());

                std::string modulePath{path.second};

                const auto pos = modulePath.find('*');
                modulePath.replace(pos, 1, moduleSubPath);

                return fs::path(modulePath);
            }
        }

        return {};
    }

    Runtime::~Runtime() {
        if (m_runtime) {
            js_std_free_handlers(m_runtime);
            JS_FreeRuntime(m_runtime);
        }
    }
}
