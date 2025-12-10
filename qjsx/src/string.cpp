//
// Created by Rob Hendriks on 09/12/2025.
//

#include "qjsx/string.h"

namespace qjsx {
    String::String(JSContext *ctx, const char *data)
        : m_context(ctx),
          m_data(data) {
    }

    const char *String::data() const {
        return m_data;
    }

    String::~String() {
        if (m_context && m_data) {
            JS_FreeCString(m_context, m_data);
        }
    }
}
