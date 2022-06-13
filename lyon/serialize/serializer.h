#ifndef __LYON_SERIALIZE_SERIALIZER_H__
#define __LYON_SERIALIZE_SERIALIZER_H__

#include "bytearray.h"
#include <bits/utility.h>
#include <iostream>
#include <list>
#include <lyon/log.h>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

//对bytearray进行封装，使常规的容器也能够序列化和反序列化
namespace lyon {

class Serializer {
public:
    typedef std::shared_ptr<Serializer> ptr;

    Serializer(bool compress = true);

    /**
     * @brief
     * 加载已经序列化的数据，注意这里加载的数据必须是已经经过序列化的，否则无法正常解码
     *
     * @param ba 序列化数据的二进制文件
     * @param compress 数据是否压缩
     */
    Serializer(ByteArray::ptr ba, bool compress);

    /**
     * @brief 加载已经序列化的数据
     *
     * @param str 序列化数据的字符串表示
     * @param compress 数据是否压缩
     */
    Serializer(const std::string &str, bool compress);

    void setPosition(size_t position);

    void clear();

    void loadFromStr(const std::string &content);

    std::string toString();

    ByteArray::ptr getByteArray() const { return m_ba; }

    //因为这里必须要用到constexpr，所以项目在这里变更为C17
    template <typename T> void writeF(const T &v) const {
        if constexpr (std::is_same_v<T, int8_t>) {
            m_ba->writeFint8(v);
        } else if constexpr (std::is_same_v<T, uint8_t>) {
            m_ba->writeFuint8(v);
        } else if constexpr (std::is_same_v<T, int16_t>) {
            m_ba->writeFint16(v);
        } else if constexpr (std::is_same_v<T, uint16_t>) {
            m_ba->writeFuint16(v);
        } else if constexpr (std::is_same_v<T, int32_t>) {
            m_ba->writeFint32(v);
        } else if constexpr (std::is_same_v<T, uint32_t>) {
            m_ba->writeFuint32(v);
        } else if constexpr (std::is_same_v<T, int64_t>) {
            m_ba->writeFint64(v);
        } else if constexpr (std::is_same_v<T, uint64_t>) {
            m_ba->writeFuint64(v);
        } else if constexpr (std::is_same_v<T, float>) {
            m_ba->writeFloat(v);
        } else if constexpr (std::is_same_v<T, double>) {
            m_ba->writeDouble(v);
        } else if constexpr (std::is_same_v<T, std::string>) {
            m_ba->writeStringF64(v);
        } else if constexpr (std::is_same_v<T, const char *>) {
            m_ba->writeStringF64(std::string(v));
        } else if constexpr (std::is_same_v<T, char *>) {
            m_ba->writeStringF64(std::string(v));
        } else {
            LYON_LOG_DEBUG(LYON_LOG_GET_LOGGER("system"))
                << "Type: " << typeid(T).name() << " not suport serializer";
        }
    }

    template <typename T> void write(const T &v) const {
        if (!m_compress) {
            writeF(v);
            return;
        }
        if constexpr (std::is_same_v<T, int8_t>) {
            m_ba->writeFint8(v);
        } else if constexpr (std::is_same_v<T, uint8_t>) {
            m_ba->writeFuint8(v);
        } else if constexpr (std::is_same_v<T, int16_t>) {
            m_ba->writeFint16(v);
        } else if constexpr (std::is_same_v<T, uint16_t>) {
            m_ba->writeFuint16(v);
        } else if constexpr (std::is_same_v<T, int32_t>) {
            m_ba->writeInt32(v);
        } else if constexpr (std::is_same_v<T, uint32_t>) {
            m_ba->writeUint32(v);
        } else if constexpr (std::is_same_v<T, int64_t>) {
            m_ba->writeInt64(v);
        } else if constexpr (std::is_same_v<T, uint64_t>) {
            m_ba->writeUint64(v);
        } else if constexpr (std::is_same_v<T, float>) {
            m_ba->writeFloat(v);
        } else if constexpr (std::is_same_v<T, double>) {
            m_ba->writeDouble(v);
        } else if constexpr (std::is_same_v<T, std::string>) {
            m_ba->writeStringVarint(v);
        } else if constexpr (std::is_same_v<T, const char *>) {
            m_ba->writeStringVarint(std::string(v));
        } else if constexpr (std::is_same_v<T, char *>) {
            m_ba->writeStringVarint(std::string(v));
        } else {
            LYON_LOG_DEBUG(LYON_LOG_GET_LOGGER("system"))
                << "Type: " << typeid(T).name() << " not suport serializer";
        }
    }

    template <typename T> void readF(T &v) const {
        if constexpr (std::is_same_v<T, int8_t>) {
            v = m_ba->readFint8();
        } else if constexpr (std::is_same_v<T, uint8_t>) {
            v = m_ba->readFuint8();
        } else if constexpr (std::is_same_v<T, int16_t>) {
            v = m_ba->readFint16();
        } else if constexpr (std::is_same_v<T, uint16_t>) {
            v = m_ba->readFuint16();
        } else if constexpr (std::is_same_v<T, int32_t>) {
            v = m_ba->readFint32();
        } else if constexpr (std::is_same_v<T, uint32_t>) {
            v = m_ba->readFuint32();
        } else if constexpr (std::is_same_v<T, int64_t>) {
            v = m_ba->readFint64();
        } else if constexpr (std::is_same_v<T, uint64_t>) {
            v = m_ba->readFuint64();
        } else if constexpr (std::is_same_v<T, float>) {
            v = m_ba->readFloat();
        } else if constexpr (std::is_same_v<T, double>) {
            v = m_ba->readDouble();
        } else if constexpr (std::is_same_v<T, std::string>) {
            v = m_ba->readStringF64();
        } else {
            LYON_LOG_DEBUG(LYON_LOG_GET_LOGGER("system"))
                << "Type: " << typeid(T).name() << " not suport serializer";
        }
    }

    template <typename T> void read(T &v) const {
        if (!m_compress) {
            readF(v);
            return;
        }
        if constexpr (std::is_same_v<T, int8_t>) {
            v = m_ba->readFint8();
        } else if constexpr (std::is_same_v<T, uint8_t>) {
            v = m_ba->readFuint8();
        } else if constexpr (std::is_same_v<T, int16_t>) {
            v = m_ba->readFint16();
        } else if constexpr (std::is_same_v<T, uint16_t>) {
            v = m_ba->readFuint16();
        } else if constexpr (std::is_same_v<T, int32_t>) {
            v = m_ba->readInt32();
        } else if constexpr (std::is_same_v<T, uint32_t>) {
            v = m_ba->readUint32();
        } else if constexpr (std::is_same_v<T, int64_t>) {
            v = m_ba->readInt64();
        } else if constexpr (std::is_same_v<T, uint64_t>) {
            v = m_ba->readUint64();
        } else if constexpr (std::is_same_v<T, float>) {
            v = m_ba->readFloat();
        } else if constexpr (std::is_same_v<T, double>) {
            v = m_ba->readDouble();
        } else if constexpr (std::is_same_v<T, std::string>) {
            v = m_ba->readStringVarint();
        } else {
            LYON_LOG_DEBUG(LYON_LOG_GET_LOGGER("system"))
                << "Type: " << typeid(T).name() << " not suport serializer";
        }
    }

private:
    bool m_compress;
    ByteArray::ptr m_ba;
};

template <typename T>
const Serializer &operator<<(const Serializer &ser, const T &v) {
    ser.write(v);
    return ser;
}

template <typename T>
const Serializer &operator>>(const Serializer &ser, T &v) {
    ser.read(v);
    return ser;
}

template <typename... Args>
const Serializer &operator<<(const Serializer &ser,
                             const std::tuple<Args...> &args) {
    const auto &unpack = [&ser]<typename Tuple, std::size_t... Index>(
                             const Tuple &t, std::index_sequence<Index...>) {
        //引用折叠。这里是二元左折叠。
        //展开后的代码为 ((ser<<get<Index0>(t))<<get<Index1>(t)...)
        (ser << ... << std::get<Index>(t));
        // (std::cout << ... << std::get<Index>(t));
    };
    unpack(args, std::index_sequence_for<Args...>{});
    return ser;
}

template <typename... Args>
const Serializer &operator>>(const Serializer &ser, std::tuple<Args...> &args) {
    const auto &pack = [&ser]<typename Tuple, std::size_t... Index>(
                           Tuple &t, std::index_sequence<Index...>) {
        (ser >> ... >> std::get<Index>(t));
    };

    pack(args, std::index_sequence_for<Args...>{});
    return ser;
}

template <typename T>
const Serializer &operator<<(const Serializer &ser, const std::vector<T> &v) {
    ser << v.size();
    for (size_t i = 0; i < v.size(); ++i) {
        ser << v[i];
    }
    return ser;
}

template <typename T>
const Serializer &operator>>(const Serializer &ser, std::vector<T> &v) {
    decltype(v.size()) size;
    ser >> size;
    for (size_t i = 0; i < size; ++i) {
        T t;
        ser >> t;
        v.push_back(t);
    }
    return ser;
}

template <typename T>
const Serializer &operator<<(const Serializer &ser, const std::list<T> &v) {
    ser << v.size();
    for (const T &v_ : v) {
        ser << v_;
    }
    return ser;
}

template <typename T>
const Serializer &operator>>(const Serializer &ser, std::list<T> &v) {
    decltype(v.size()) size;
    ser >> size;
    for (size_t i = 0; i < size; ++i) {
        T t;
        ser >> t;
        v.push_back(t);
    }
    return ser;
}

template <typename F, typename S>
const Serializer &operator<<(const Serializer &ser, const std::pair<F, S> &v) {
    ser << v.first << v.second;
    return ser;
}

template <typename F, typename S>
const Serializer &operator>>(const Serializer &ser, const std::pair<F, S> &v) {
    ser >> v.first >> v.second;
    return ser;
}

template <typename K, typename V>
const Serializer &operator<<(const Serializer &ser, const std::map<K, V> &v) {
    ser << v.size();
    for (auto &i : v) {
        ser << i;
    }
    return ser;
}

template <typename K, typename V>
const Serializer &operator>>(const Serializer &ser, std::map<K, V> &v) {
    decltype(v.size()) size;
    ser >> size;
    for (size_t i = 0; i < size; i++) {
        std::pair<K, V> p;
        ser >> p;
        v.insert(p);
    }
    return ser;
}

} // namespace lyon

#endif
