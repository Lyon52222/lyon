#ifndef __LYON_CONFIG_H__
#define __LYON_CONFIG_H__

#include "log.h"
#include <boost/lexical_cast.hpp>
#include <exception>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <util.h>
#include <yaml-cpp/yaml.h>
namespace lyon {
/**
 * @brief 配置项基类，包含配置名，和配置描述
 */
class ConfigVarBase {
  public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string &name, const std::string &description = "")
        : m_name(name), m_description(description){};
    virtual ~ConfigVarBase(){};

    const std::string &getName() const { return m_name; };
    const std::string &getDescription() const { return m_description; };

    virtual std::string toString() = 0;
    virtual bool fromString(const std::string &str) = 0;

  protected:
    std::string m_name;
    std::string m_description;
};

/**
 * @brief 配置项模版类，增加了配置值（模版值）
 *
 * @tparam T 配置项值的类型
 */
template <class T> class ConfigVar : public ConfigVarBase {
  public:
    typedef std::shared_ptr<ConfigVar> ptr;
    ConfigVar(const std::string &name, const T &default_value,
              const std::string &description = "")
        : ConfigVarBase(name, description), m_val(default_value) {}
    std::string toString() override {
        try {
            return boost::lexical_cast<std::string>(m_val);
        } catch (std::exception &e) {
            LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
                << "ConfigVar::toString exception" << e.what()
                << "convert: " << typeid(m_val).name() << "to string";
        }
        return "";
    }

    bool fromString(const std::string &str) override {
        try {
            m_val = boost ::lexical_cast<T>(str);
            return true;
        } catch (std::exception &e) {
            LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
                << "ConfigVar::fromString exception" << e.what()
                << "convert: string to " << typeid(m_val).name();

            return false;
        }
    }

  private:
    T m_val;
};

class Config {
  public:
    typedef std::shared_ptr<Config> ptr;
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;

    template <class T>
    static typename ConfigVar<T>::ptr
    AddConfig(const std::string &name, const T &default_value,
              const std::string &description) {

        if (!CheckName(name)) {
            // throw std::invalid_argument(name);
            return nullptr;
        }
        auto tmp = Lookup<T>(name);
        if (tmp) {
            LYON_LOG_INFO(LYON_LOG_GET_ROOT())
                << "Config: " << name << " is exists";
            return tmp;
        }

        typename ConfigVar<T>::ptr v(
            new ConfigVar<T>(name, default_value, description));
        GetConfigs()[name] = v;
        return v;
    }

    template <class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string &name) {
        auto itr = GetConfigs().find(name);
        if (itr == GetConfigs().end()) {
            LYON_LOG_INFO(LYON_LOG_GET_ROOT())
                << "Config: " << name << " is not exists";
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T>>(itr->second);
    }

    static void LoadFromYaml(const YAML::Node &root);

    static void LoadFromConfigFile(const std::string &path);

    static bool CheckName(const std::string &name) {

        if (!IsConfigNameAvilable(name)) {
            LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
                << "Name : " << name
                << " is not avilable : Config name should be named with "
                   "[a-zA-Z0-9_]";

            return false;
        }
        return true;
    }

    // static ConfigVarMap m_configs;
    // 因为类的静态成员变量需要在类的外部进行初始化。
    /**
     * @brief 返回配置项
     *
     * @return 配置项
     */
    static ConfigVarMap &GetConfigs() {
        static ConfigVarMap m_configs;
        return m_configs;
    }

  private:
};

} // namespace lyon
#endif // !__LYON_CONFIG_H__
