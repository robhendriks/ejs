//
// Created by Rob Hendriks on 09/12/2025.
//

#include "qjsx/context.h"

#include <string>
#include <sstream>
#include <quickjs-libc.h>

namespace qjsx {
    inline std::string getEntryScript(const std::string_view moduleName) {
        std::stringstream ss;
        ss << "import * as m from '" << moduleName << "'; export default m;";
        return ss.str();
    }

    Context::Context(const Runtime &rt, void *userData)
        : m_context(JS_NewContext(rt.m_runtime)),
          m_userData(userData) {
        js_init_module_std(m_context, "std");
        js_init_module_os(m_context, "os");
        js_std_add_helpers(m_context, 0, nullptr);

        JS_SetContextOpaque(m_context, this);
    }

    Value Context::eval(const std::string_view source, const std::string_view filename) const {
        const JSValue result = JS_Eval(
            m_context,
            source.data(),
            source.length(),
            filename.data(),
            0
        );

        return Value{m_context, result};
    }

    Value Context::evalModule(const std::string_view moduleName) const {
        const auto entryScript = getEntryScript(moduleName);

        const JSValue result = JS_Eval(
            m_context,
            entryScript.data(),
            entryScript.length(),
            "<entry>",
            JS_EVAL_TYPE_MODULE
        );

        return Value{m_context, result};
    }


    JSContext *Context::ptr() const {
        return m_context;
    }

    Context::operator JSContext *() const {
        return m_context;
    }

    Value Context::getModule(const std::string_view moduleName) const {
        const auto it = m_modules.find(std::string(moduleName));

        if (it == m_modules.end()) {
            return Value{nullptr, JS_UNINITIALIZED};
        }

        return Value{m_context, JS_GetModuleNamespace(m_context, it->second)};
    }

    Context::ModuleMap &Context::getModules() {
        return m_modules;
    }

    const Context::ModuleMap &Context::getModules() const {
        return m_modules;
    }

    Context::~Context() {
        if (m_context) {
            JS_FreeContext(m_context);
        }
    }
}
