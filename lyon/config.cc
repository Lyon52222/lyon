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

    numbers.emplace_back(prefix, node);
    if (node.IsMap()) {
        for (auto itr = node.begin(); itr != node.end(); itr++) {
            ListAllYamlNumbers(prefix.empty()
                                   ? itr->first.Scalar()
                                   : prefix + "." + itr->first.Scalar(),
                               itr->second, numbers);
        }
    }
}

void Config::LoadFromYaml(const YAML::Node &root) {
    std::list<std::pair<std::string, YAML::Node>> numbers;
    // ListAllYamlNumbers("", root, numbers);

    // YAML::Node t = YAML::LoadFile("test.txt");
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
        }
    }
}

void Config::LoadFromConfigFile(const std::string &path) {
    std::cout << path << std::endl;
    // YAML::Node root = YAML::LoadFile(path);
    // try {
    //     LYON_LOG_INFO(LYON_LOG_GET_ROOT())
    //         << "LoadConfig: " << path << "Success!";
    // } catch (...) {
    //     LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
    //         << "LoadConfig: " << path << "Failed!";
    // }
}

} // namespace lyon
