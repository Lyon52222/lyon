#include "lyon/config.h"
#include "lyon/log.h"
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>

const std::string YAMLType[5] = {"Undefined", "Null", "Scalar", "Sequence",
                                 "Map"};

void printYAML(const YAML::Node &node, int level) {
    if (node.IsNull()) {
        LYON_LOG_INFO(LYON_LOG_GET_ROOT())
            << std::string(level * 4, ' ') << "NULL - " << YAMLType[node.Type()]
            << " - " << level;
    } else if (node.IsScalar()) {
        LYON_LOG_INFO(LYON_LOG_GET_ROOT())
            << std::string(level * 4, ' ') << node.Scalar() << " - "
            << YAMLType[node.Type()] << " - " << level;
    } else if (node.IsSequence()) {
        for (size_t i = 0; i < node.size(); i++) {
            LYON_LOG_INFO(LYON_LOG_GET_ROOT())
                << std::string(level * 4, ' ') << i << " - "
                << YAMLType[node[i].Type()] << " - " << level;
            printYAML(node[i], level + 1);
        }
    } else if (node.IsMap()) {
        for (auto itr = node.begin(); itr != node.end(); itr++) {
            LYON_LOG_INFO(LYON_LOG_GET_ROOT())
                << std::string(level * 4, ' ') << itr->first << " - "
                << YAMLType[itr->second.Type()] << " - " << level;
            printYAML(itr->second, level + 1);
        }
    }
}

int main() {
    lyon::Config::AddConfig<std::string>("ip", "127.0.0.1", "ipv4_addr");

    std::cout << lyon::Config::Lookup<std::string>("ip")->toString()
              << std::endl;
    std::cout << "------------------------------" << std::endl;
    YAML::Node root =
        YAML::LoadFile("/Users/admin/Code/C++/lyon/bin/conf/log.yml");
    printYAML(root, 0);
    std::cout << "------------------------------" << std::endl;

    lyon::Config::LoadFromConfigFile(
        "/Users/admin/Code/C++/lyon/bin/conf/log.yml");
    std::cout << lyon::Config::Lookup<std::string>("ip")->toString()
              << std::endl;
    std::cout << lyon::Config::Lookup<std::string>("author.name")->toString()
              << std::endl;

    return 0;
}
