//
// Created by Rob Hendriks on 09/12/2025.
//

#pragma once

#include "string.h"
#include <string_view>
#include <quickjs.h>

namespace qjsx {
    class Context;

    class Value final {
        friend class Context;

    protected:
        Value(JSContext *context, JSValue value);

    public:
        Value(const Value &other);

        Value(Value &&other) noexcept;

        [[nodiscard]] bool isArray() const;

        [[nodiscard]] bool isBoolean() const;

        [[nodiscard]] bool isException() const;

        [[nodiscard]] bool isNumber() const;

        [[nodiscard]] bool isObject() const;

        [[nodiscard]] bool isString() const;

        [[nodiscard]] bool isUndefined() const;

        [[nodiscard]] bool isNull() const;

        [[nodiscard]] bool isFunction() const;

        template<typename T>
        [[nodiscard]] T as() const;

        void printException() const;

        Value operator()() const;

        Value operator[](std::string_view propertyName) const;

        Value operator[](size_t index) const;

        Value &operator=(const Value &other) = delete;

        Value &operator=(Value &&other) noexcept = delete;

        ~Value();

    private:
        JSContext *m_context;
        JSValue m_value;
    };

    template<>
    bool Value::as<bool>() const;

    template<>
    int Value::as<int>() const;

    template<>
    float Value::as<float>() const;

    template<>
    String Value::as<String>() const;
}
