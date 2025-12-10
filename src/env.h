//
// Created by Rob Hendriks on 10/12/2025.
//

#pragma once

#include <string>
#include <cstdlib>

namespace ejs::env {
    inline std::string getVariable(const std::string_view name) {
#ifdef _WIN32
        char *buf = nullptr;
        size_t sz = 0;
        if (_dupenv_s(&buf, &sz, name.data()) == 0 && buf != nullptr) {
            std::string variable{buf};
            free(buf);
            return variable;
        }
#endif

        return {};
    }
}
