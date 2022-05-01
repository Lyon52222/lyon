#include "config.h"
#include <algorithm>
#include <sstream>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/parser.h>

namespace lyon {

static void
ListAllYamlNumbers(const std::string &prefix, const YAML::Node &node,
                   std::list<std::pair<std::string, YAML::Node>> &numbers) {
    if (!Config::CheckName(prefix)) {
        return;
    }
    // NOTE: 这里只处理了常量类型。
    if (node.IsScalar()) {
        // std::cout << prefix << " : " << node << std::endl;
        numbers.emplace_back(prefix, node);
    }
    if (node.IsMap()) {
        for (auto itr = node.begin(); itr != node.end(); itr++) {
            ListAllYamlNumbers(prefix.empty()
                                   ? itr->first.Scalar()
                                   : prefix + "." + itr->first.Scalar(),
                               itr->second, numbers);
        }
    } else if (node.IsSequence()) {
        for (size_t i = 0; i < node.size(); i++) {
            ListAllYamlNumbers(prefix, node[i], numbers);
        }
    }
}

void Config::LoadFromYaml(const YAML::Node &root) {
    std::list<std::pair<std::string, YAML::Node>> numbers;
    ListAllYamlNumbers("", root, numbers);

    for (auto &number : numbers) {
        std::string key = number.first;
        if (key.empty()) {
            continue;
        }
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);

        auto conf = GetConfigs().find(key);
        if (conf != GetConfigs().end()) {
            auto &val = number.second;
            if (val.IsScalar()) {
                conf->second->fromString(val.Scalar());
            } else {
                std::stringstream ss;
                ss << val;
                conf->second->fromString(ss.str());
            }
        } else {
            auto &val = number.second;
            if (val.IsScalar()) {
                AddConfig(key, val.Scalar(), key);
            } else {
                std::stringstream ss;
                ss << val;

                AddConfig(key, ss.str(), key);
            }
        }
    }
}

void Config::LoadFromConfigFile(const std::string &path) {
    YAML::Node root = YAML::LoadFile(path);
    LoadFromYaml(root);
    try {
        LYON_LOG_INFO(LYON_LOG_GET_ROOT())
            << "LoadConfig: " << path << "Success!";
    } catch (...) {
        LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
            << "LoadConfig: " << path << "Failed!";
    }
}

} // namespace lyon
