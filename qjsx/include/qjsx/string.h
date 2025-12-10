//
// Created by Rob Hendriks on 09/12/2025.
//

#pragma once

#include <quickjs.h>

namespace qjsx {
    class String {
        friend class Value;

    public:
        String(JSContext *ctx, const char *data);

        String(const String &other) = delete;

        String(String &&other) = delete;

        String &operator=(const String &) = delete;

        String &operator=(String &&) = delete;

        [[nodiscard]] const char *data() const;

        ~String();

    private:
        JSContext *m_context;
        const char *m_data;
    };
}
