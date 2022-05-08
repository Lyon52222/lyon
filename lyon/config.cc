#include "config.h"
#include "log.h"
#include <algorithm>
#include <sstream>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/parser.h>

namespace lyon {

// static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");
static Logger::ptr g_logger = LYON_LOG_GET_ROOT();

static void
ListAllYamlNumbers(const std::string &prefix, const YAML::Node &node,
                   std::list<std::pair<std::string, YAML::Node>> &numbers) {
    if (!Config::CheckName(prefix)) {
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
        // INFO:这里觉得是否将为约定的配置项加入到配置中,这里构建时，无法确定配置项具体的类型。只能将其保存为string类型。
        else {
            auto &val = number.second;
            if (val.IsScalar()) {
                SetConfig(key, val.Scalar(), key);
            } else {
                std::stringstream ss;
                ss << val;

                SetConfig(key, ss.str(), key);
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
