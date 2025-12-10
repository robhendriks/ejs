#pragma once

#include "plugin.h"

#include <yaml-cpp/yaml.h>

template<>
struct YAML::convert<ejs::PluginInfo> {
    static bool decode(const Node &node, ejs::PluginInfo &rhs) {
        if (!node.IsMap()) {
            return false;
        }

        // TODO: validation

        rhs.id = node["id"].as<std::string>();
        rhs.name = node["name"].as<std::string>();

        return true;
    }
};
