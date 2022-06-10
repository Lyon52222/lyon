#include "config.h"
#include "log.h"
#include <algorithm>
#include <sstream>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/parser.h>

namespace lyon {

// static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");
static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

static void
ListAllYamlNumbers(const std::string &prefix, const YAML::Node &node,
                   std::list<std::pair<std::string, YAML::Node>> &numbers) {
    if (!lyon::Config::CheckName(prefix)) {
        return;
    }
    if (node.IsScalar()) {
        numbers.emplace_back(prefix, node);
    } else if (node.IsSequence()) {
        numbers.emplace_back(prefix, node);
    } else if (node.IsMap()) {
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
    ListAllYamlNumbers("", root, numbers);

    for (auto &number : numbers) {
        std::string key = number.first;
        if (key.empty()) {
            continue;
        }
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);

        //约定优先于配置，只有当程序中约定了配置时，配置文件才有效
        if (auto conf = GetConfigs().find(key); conf != GetConfigs().end()) {
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
    try {
        YAML::Node root = YAML::LoadFile(path);
        LYON_LOG_INFO(g_logger) << "LoadConfig: " << path << " Success!";
        LoadFromYaml(root);
    } catch (...) {
        LYON_LOG_ERROR(g_logger) << "LoadConfig: " << path << " Fail!";
    }
}

bool Config::CheckName(const std::string &name) {
    if (!IsConfigNameAvilable(name)) {
        LYON_LOG_ERROR(g_logger)
            << "Name : " << name
            << " is not avilable : Config name should be named with "
               "[a-zA-Z0-9_]";

        return false;
    }
    return true;
}

void Config::VisitConfigs(std::function<void(ConfigVarBase::ptr)> f) {
    RWMutexType::RDLock lock(GetRWMutex());
    for (auto &config : GetConfigs()) {
        f(config.second);
    }
}

} // namespace lyon
