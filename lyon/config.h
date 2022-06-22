#ifndef __LYON_CONFIG_H__
#define __LYON_CONFIG_H__

#include "log.h"
#include "mutex.h"
#include "util.h"
#include <boost/lexical_cast.hpp>
#include <exception>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/node/type.h>
#include <yaml-cpp/yaml.h>
namespace lyon {

/**
 * @brief 两种类型的转换
 *
 * @tparam F 原本类型
 * @tparam T 目标类型
 */
template <typename F, typename T> class LexicalCast {
public:
    T operator()(const F &f) { return boost::lexical_cast<T>(f); };
};

/**
 * @brief 将由T类型构成的vector转化为string。  使用YAML::Load来完成中间的转化
 *
 * @tparam T vector类型
 */
template <typename T> class LexicalCast<std::vector<T>, std::string> {
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
template <typename T> class LexicalCast<std::string, std::vector<T>> {
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

template <typename T> class LexicalCast<std::list<T>, std::string> {
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

template <typename T> class LexicalCast<std::string, std::list<T>> {
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

/**
 * @brief 从T类型的字典转化为YAML格式的字符串
 *
 * @tparam T 字典的类型
 */
template <typename T> class LexicalCast<std::map<std::string, T>, std::string> {
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

/**
 * @brief 从YAML格式的字符串转化为T类型的字典。
 *
 * @tparam T 字典的类型
 */
template <typename T> class LexicalCast<std::string, std::map<std::string, T>> {
public:
    std::map<std::string, T> operator()(const std::string &s) {
        YAML::Node node = YAML::Load(s);
        typename std::map<std::string, T> m;
        for (auto itr = node.begin(); itr != node.end(); itr++) {
            std::stringstream ss;
            ss << itr->second;
            m[itr->first.Scalar()] = LexicalCast<std::string, T>()(ss.str());
        }
        return m;
    }
};

template <typename T> class LexicalCast<std::set<T>, std::string> {
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

template <typename T> class LexicalCast<std::string, std::set<T>> {
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
template <typename T, typename FromStr = LexicalCast<std::string, T>,
          typename ToStr = LexicalCast<T, std::string>>
class ConfigVar : public ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void(const T &old_val, const T &new_val)>
        on_change_cb;
    typedef RWMutex RWMutexType;

    ConfigVar(const std::string &name, const T &default_value,
              const std::string &description = "")
        : ConfigVarBase(name, description), m_val(default_value) {}

    /**
     * @brief 将配置项转换为Yaml string
     *
     */
    std::string toString() override {
        try {
            return ToStr()(getVal());
        } catch (std::exception &e) {
            LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
                << "ConfigVar::toString exception " << e.what()
                << " convert: " << typeid(m_val).name() << " to string";
        }
        return "";
    }

    /**
     * @brief 从Yaml string 构建配置项
     *
     * @param str Yaml string
     * @return 构建是否成功
     */
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
        RWMutexType::RDLock lock(m_mutex);
        return m_val;
    }

    void setVal(const T &val) {
        {
            RWMutexType::RDLock lock(m_mutex);
            if (val == m_val) {
                return;
            }
            onChange(m_val, val);
        }

        RWMutexType::WRLock lock(m_mutex);
        m_val = val;
    }

    /**
     * @brief 设置配置项发生改变时的回调函数
     *
     * @param cb 回调函数
     * @return 回调函数唯一标识
     */
    uint32_t addOnChange(const on_change_cb cb) {
        static uint32_t cb_id = 0;
        RWMutexType::WRLock lock(m_mutex);
        m_cbs[cb_id++] = cb;
        return cb_id - 1;
    }

    bool delOnChange(uint32_t cb_id) {
        RWMutexType::WRLock lock(m_mutex);
        if (m_cbs.find(cb_id) == m_cbs.end()) {
            return false;
        } else {
            m_cbs.erase(cb_id);
            return true;
        }
    }

    void clearOnChange() {
        RWMutexType::WRLock lock(m_mutex);
        m_cbs.clear();
    }

private:
    T m_val;
    std::map<uint32_t, on_change_cb> m_cbs;

    void onChange(const T &old_val, const T &new_val) {
        for (auto itr = m_cbs.begin(); itr != m_cbs.end(); itr++) {
            itr->second(old_val, new_val);
        }
    }

    RWMutexType m_mutex;
};

class Config {
public:
    typedef std::shared_ptr<Config> ptr;
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;
    typedef RWMutex RWMutexType;

    /**
     * @brief 添加配置项
     *
     * @tparam T 配置项类型
     * @param name 配置名
     * @param default_value 配置项值
     * @param description 配置项描述
     */
    template <typename T>
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
        RWMutexType::WRLock lock(GetRWMutex());
        GetConfigs()[name] = v;
        return v;
    }

    /**
     * @brief 检查配置项是否存在并返回
     *
     * @tparam T 配置项类型
     * @param name 配置名
     */
    template <typename T>
    static typename ConfigVar<T>::ptr Lookup(const std::string &name) {
        RWMutexType::RDLock lock(GetRWMutex());
        auto itr = GetConfigs().find(name);
        if (itr == GetConfigs().end()) {
            // LYON_LOG_INFO(LYON_LOG_GET_ROOT())
            //     << "Config: " << name << " is not exists";
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T>>(itr->second);
    }

    /**
     * @brief 从Yaml节点加载配置项
     *
     * @param root Yaml节点
     */
    static void LoadFromYaml(const YAML::Node &root);

    /**
     * @brief 从配置文件加载配置项
     *
     * @param path 配置文件路径
     */
    static void LoadFromConfigFile(const std::string &path);

    /**
     * @brief 检查配置名的合法性
     *
     * @param name 配置名
     * @return 配置名是否合法
     */
    static bool CheckName(const std::string &name);

    /**
     * @brief 使用访问函数f对所有配置项进行访问
     *
     * @param f 访问函数
     */
    static void VisitConfigs(std::function<void(ConfigVarBase::ptr)> f);

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

    static RWMutexType &GetRWMutex() {
        static RWMutexType m_mutex;
        return m_mutex;
    }

private:
    RWMutexType m_mutex;
};

} // namespace lyon
#endif // !__LYON_CONFIG_H__
