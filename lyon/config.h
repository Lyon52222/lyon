#ifndef __LYON_CONFIG_H__
#define __LYON_CONFIG_H__

#include "log.h"
#include "util.h"
#include <boost/lexical_cast.hpp>
#include <exception>
#include <memory>
#include <mutex.h>
#include <sstream>
#include <stdexcept>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/node/type.h>
#include <yaml-cpp/yaml.h>
namespace lyon {
// TODO: add lock

/**
 * @brief 两种类型的转换
 *
 * @tparam F 原本类型
 * @tparam T 目标类型
 */
template <class F, class T> class LexicalCast {
public:
    T operator()(const F &f) { return boost::lexical_cast<T>(f); };
};

/**
 * @brief 将由T类型构成的vector转化为string。  使用YAML::Load来完成中间的转化
 *
 * @tparam T vector类型
 */
template <class T> class LexicalCast<std::vector<T>, std::string> {
public:
    std::string operator()(const std::vector<T> &v) {
        YAML::Node node(YAML::NodeType::Sequence);
        for (auto &i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief 将string转化为由T类型构成的vector。 使用YAML::Load来完成中间的转化
 *
 * @tparam T vector类型
 */
template <class T> class LexicalCast<std::string, std::vector<T>> {
public:
    std::vector<T> operator()(const std::string &s) {
        YAML::Node node = YAML::Load(s);
        typename std::vector<T> vec;
        for (size_t i = 0; i < node.size(); i++) {
            std::stringstream ss;
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

template <class T> class LexicalCast<std::list<T>, std::string> {
public:
    std::string operator()(const std::list<T> &l) {
        YAML::Node node(YAML::NodeType::Sequence);
        for (auto &i : l) {
            node.push_back(LexicalCast<T, std::string>()(i));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T> class LexicalCast<std::string, std::list<T>> {
public:
    std::list<T> operator()(const std::string &s) {
        YAML::Node node = YAML::Load(s);
        typename std::list<T> l;
        for (size_t i = 0; i < node.size(); i++) {
            std::stringstream ss;
            ss << node[i];
            l.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return l;
    }
};

template <class T> class LexicalCast<std::map<std::string, T>, std::string> {
public:
    std::string operator()(const std::map<std::string, T> &m) {
        YAML::Node node(YAML::NodeType::Map);
        for (auto itr = m.begin(); itr != m.end(); itr++) {
            node.force_insert(itr->first,
                              LexicalCast<T, std::string>()(itr->second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T> class LexicalCast<std::string, std::map<std::string, T>> {
public:
    std::map<std::string, T> operator()(const std::string &s) {
        YAML::Node node = YAML::Load(s);
        typename std::map<std::string, T> m;
        for (auto itr = node.begin(); itr != m.end(); itr++) {
            std::stringstream ss;
            ss << itr->second;
            m[itr->first] = LexicalCast<std::string, T>()(ss.str());
        }
        return m;
    }
};

template <class T> class LexicalCast<std::set<T>, std::string> {
public:
    std::string operator()(const std::set<T> &s) {
        YAML::Node node;
        for (auto itr = s.begin(); itr != s.end(); itr++) {
            node.push_back(LexicalCast<T, std::string>()(*itr));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T> class LexicalCast<std::string, std::set<T>> {
public:
    std::set<T> operator()(const std::string &s) {
        YAML::Node node = YAML::Load(s);
        typename std::set<T> st;
        for (auto itr = node.begin(); itr != node.end(); itr++) {
            std::stringstream ss;
            ss << (*itr);
            st.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return st;
    }
};

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
template <class T, class FromStr = LexicalCast<std::string, T>,
          class ToStr = LexicalCast<T, std::string>>
class ConfigVar : public ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void(const T &old_val, const T &new_val)>
        on_change_cb;
    typedef RWMutex MutexType;

    ConfigVar(const std::string &name, const T &default_value,
              const std::string &description = "")
        : ConfigVarBase(name, description), m_val(default_value) {}
    std::string toString() override {
        try {
            return ToStr()(m_val);
        } catch (std::exception &e) {
            LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
                << "ConfigVar::toString exception " << e.what()
                << " convert: " << typeid(m_val).name() << " to string";
        }
        return "";
    }

    bool fromString(const std::string &str) override {
        try {
            setVal(FromStr()(str));
            return true;
        } catch (std::exception &e) {
            LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
                << "ConfigVar::fromString exception " << e.what()
                << " convert: string to " << typeid(m_val).name();

            return false;
        }
    }

    const T &getVal() {
        MutexType::RDLock lock(m_mutex);
        return m_val;
    }

    void setVal(const T &val) {
        if (val != m_val) {
            onChange(m_val, val);
            m_val = val;
        }
    }

    uint32_t addOnChange(const on_change_cb cb) {
        static uint32_t cb_id = 0;
        m_cbs[cb_id++] = cb;
        return cb_id - 1;
    }

    bool delOnChange(uint32_t cb_id) {
        if (m_cbs.find(cb_id) == m_cbs.end()) {
            return false;
        } else {
            m_cbs.erase(cb_id);
            return true;
        }
    }

    void clearOnChange() { m_cbs.clear(); }

private:
    T m_val;
    std::map<uint32_t, on_change_cb> m_cbs;

    void onChange(const T &old_val, const T &new_val) {
        for (auto itr = m_cbs.begin(); itr != m_cbs.end(); itr++) {
            itr->second(old_val, new_val);
        }
    }

    MutexType m_mutex;
};

class Config {
public:
    typedef std::shared_ptr<Config> ptr;
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;

    template <class T>
    static typename ConfigVar<T>::ptr
    SetConfig(const std::string &name, const T &default_value,
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
            // LYON_LOG_INFO(LYON_LOG_GET_ROOT())
            //     << "Config: " << name << " is not exists";
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T>>(itr->second);
    }

    static void LoadFromYaml(const YAML::Node &root);

    static void LoadFromConfigFile(const std::string &path);

    static bool CheckName(const std::string &name);

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
