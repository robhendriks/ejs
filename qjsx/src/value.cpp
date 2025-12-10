//
// Created by Rob Hendriks on 09/12/2025.
//

#include "qjsx/value.h"

#include <quickjs-libc.h>
#include <stdexcept>
#include <utility>

namespace qjsx {
    Value::Value(JSContext *context, const JSValue value)
        : m_context(context),
          m_value(value) {
    }

    Value::Value(const Value &other)
        : m_context(other.m_context),
          m_value(JS_DupValue(other.m_context, other.m_value)) {
    }

    Value::Value(Value &&other) noexcept
        : m_context(std::exchange(other.m_context, nullptr)),
          m_value(std::exchange(other.m_value, JS_UNINITIALIZED)) {
    }

    bool Value::isArray() const {
        return JS_IsArray(m_value);
    }

    bool Value::isBoolean() const {
        return JS_IsBool(m_value);
    }

    bool Value::isException() const {
        return JS_IsException(m_value);
    }

    bool Value::isNumber() const {
        return JS_IsNumber(m_value);
    }

    bool Value::isObject() const {
        return JS_IsObject(m_value);
    }

    bool Value::isString() const {
        return JS_IsString(m_value);
    }

    bool Value::isUndefined() const {
        return JS_IsUndefined(m_value);
    }

    bool Value::isNull() const {
        return JS_IsNull(m_value);
    }

    bool Value::isFunction() const {
        return JS_IsFunction(m_context, m_value);
    }

    template<typename T>
    T Value::as() const {
        throw std::runtime_error("unsupported type conversion");
    }

    void Value::printException() const {
        js_std_dump_error(m_context);
    }

    Value Value::operator()() const {
        return Value{m_context, JS_Call(m_context, m_value, JS_UNDEFINED, 0, nullptr)};
    }

    Value Value::operator[](const std::string_view propertyName) const {
        return Value{
            m_context,
            JS_GetPropertyStr(m_context, m_value, propertyName.data()),
        };
    }

    Value Value::operator[](const size_t index) const {
        return Value{
            m_context,
            JS_GetPropertyInt64(m_context, m_value, index),
        };
    }

    Value::~Value() {
        if (m_context && !JS_IsUninitialized(m_value)) {
            JS_FreeValue(m_context, m_value);
        }
    }

    template<>
    bool Value::as<bool>() const {
        return JS_VALUE_GET_BOOL(m_value);
    }

    template<>
    int Value::as<int>() const {
        return JS_VALUE_GET_INT(m_value);
    }

    template<>
    float Value::as<float>() const {
        return JS_VALUE_GET_FLOAT64(m_value);
    }

    template<>
    String Value::as<String>() const {
        return String{
            m_context,
            JS_ToCString(m_context, m_value),
        };
    }
}
