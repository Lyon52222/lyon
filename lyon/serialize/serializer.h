#ifndef __LYON_SERIALIZE_SERIALIZER_H__
#define __LYON_SERIALIZE_SERIALIZER_H__

#include "bytearray.h"
#include <iostream>
#include <list>
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

    Serializer(size_t ba_size = 4096, bool fix = false);

    void setPosition(size_t position);

    //因为这里必须要用到constexpr，所以项目在这里变更为C17
    template <class T> void writeF(const T &v) {
        if constexpr (std::is_same<T, int8_t>::value) {
            m_ba->writeFint8(v);
        } else if constexpr (std::is_same<T, uint8_t>::value) {
            m_ba->writeFuint8(v);
        } else if constexpr (std::is_same<T, int16_t>::value) {
            m_ba->writeFint16(v);
        } else if constexpr (std::is_same<T, uint16_t>::value) {
            m_ba->writeFuint16(v);
        } else if constexpr (std::is_same<T, int32_t>::value) {
            m_ba->writeFint32(v);
        } else if constexpr (std::is_same<T, uint32_t>::value) {
            m_ba->writeFuint32(v);
        } else if constexpr (std::is_same<T, int64_t>::value) {
            m_ba->writeFint64(v);
        } else if constexpr (std::is_same<T, uint64_t>::value) {
            m_ba->writeFuint64(v);
        } else if constexpr (std::is_same<T, float>::value) {
            m_ba->writeFloat(v);
        } else if constexpr (std::is_same<T, double>::value) {
            m_ba->writeDouble(v);
        } else if constexpr (std::is_same<T, std::string>::value) {
            m_ba->writeStringVarint(v);
        }
    }

    template <class T> void write(const T &v) {
        if (m_fix) {
            writeF(v);
            return;
        }
        if constexpr (std::is_same<T, int8_t>::value) {
            m_ba->writeFint8(v);
        } else if constexpr (std::is_same<T, uint8_t>::value) {
            m_ba->writeFuint8(v);
        } else if constexpr (std::is_same<T, int16_t>::value) {
            m_ba->writeFint16(v);
        } else if constexpr (std::is_same<T, uint16_t>::value) {
            m_ba->writeFuint16(v);
        } else if constexpr (std::is_same<T, int32_t>::value) {
            m_ba->writeInt32(v);
        } else if constexpr (std::is_same<T, uint32_t>::value) {
            m_ba->writeUint32(v);
        } else if constexpr (std::is_same<T, int64_t>::value) {
            m_ba->writeInt64(v);
        } else if constexpr (std::is_same<T, uint64_t>::value) {
            m_ba->writeUint64(v);
        } else if constexpr (std::is_same<T, float>::value) {
            m_ba->writeFloat(v);
        } else if constexpr (std::is_same<T, double>::value) {
            m_ba->writeDouble(v);
        } else if constexpr (std::is_same<T, std::string>::value) {
            m_ba->writeStringVarint(v);
        }
    }

    template <class T> void readF(T &v) {
        if constexpr (std::is_same<T, int8_t>::value) {
            v = m_ba->readFint8();
        } else if constexpr (std::is_same<T, uint8_t>::value) {
            v = m_ba->readFuint8();
        } else if constexpr (std::is_same<T, int16_t>::value) {
            v = m_ba->readFint16();
        } else if constexpr (std::is_same<T, uint16_t>::value) {
            v = m_ba->readFuint16();
        } else if constexpr (std::is_same<T, int32_t>::value) {
            v = m_ba->readFint32();
        } else if constexpr (std::is_same<T, uint32_t>::value) {
            v = m_ba->readFuint32();
        } else if constexpr (std::is_same<T, int64_t>::value) {
            v = m_ba->readFint64();
        } else if constexpr (std::is_same<T, uint64_t>::value) {
            v = m_ba->readFuint64();
        } else if constexpr (std::is_same<T, float>::value) {
            v = m_ba->readFloat();
        } else if constexpr (std::is_same<T, double>::value) {
            v = m_ba->readDouble();
        } else if constexpr (std::is_same<T, std::string>::value) {
            v = m_ba->readStringVarint();
        }
    }

    template <class T> void read(T &v) {
        if (m_fix) {
            readF(v);
            return;
        }
        if constexpr (std::is_same<T, int8_t>::value) {
            v = m_ba->readFint8();
        } else if constexpr (std::is_same<T, uint8_t>::value) {
            v = m_ba->readFuint8();
        } else if constexpr (std::is_same<T, int16_t>::value) {
            v = m_ba->readFint16();
        } else if constexpr (std::is_same<T, uint16_t>::value) {
            v = m_ba->readFuint16();
        } else if constexpr (std::is_same<T, int32_t>::value) {
            v = m_ba->readInt32();
        } else if constexpr (std::is_same<T, uint32_t>::value) {
            v = m_ba->readUint32();
        } else if constexpr (std::is_same<T, int64_t>::value) {
            v = m_ba->readInt64();
        } else if constexpr (std::is_same<T, uint64_t>::value) {
            v = m_ba->readUint64();
        } else if constexpr (std::is_same<T, float>::value) {
            v = m_ba->readFloat();
        } else if constexpr (std::is_same<T, double>::value) {
            v = m_ba->readDouble();
        } else if constexpr (std::is_same<T, std::string>::value) {
            v = m_ba->readStringVarint();
        }
    }

private:
    bool m_fix;
    ByteArray::ptr m_ba;
};

template <class T> const Serializer &operator<<(Serializer &ser, const T &v) {
    ser.write(v);
    return ser;
}

template <class T> const Serializer &operator>>(Serializer &ser, T &v) {
    ser.read(v);
    return ser;
}

template <class T>
const Serializer &operator<<(Serializer &ser, const std::vector<T> &v) {
    ser << v.size();
    for (size_t i = 0; i < v.size(); ++i) {
        ser << v[i];
    }
    return ser;
}

template <class T>
const Serializer &operator>>(Serializer &ser, std::vector<T> &v) {
    decltype(v.size()) size;
    ser >> size;
    for (size_t i = 0; i < size; ++i) {
        T t;
        ser >> t;
        v.push_back(t);
    }
    return ser;
}

template <class T>
const Serializer &operator<<(Serializer &ser, const std::list<T> &v) {
    ser << v.size();
    for (size_t i = 0; i < v.size(); ++i) {
        ser << v[i];
    }
    return ser;
}

template <class T>
const Serializer &operator>>(Serializer &ser, std::list<T> &v) {
    decltype(v.size()) size;
    ser >> size;
    for (size_t i = 0; i < size; ++i) {
        T t;
        ser >> t;
        v.push_back(t);
    }
    return ser;
}

template <class F, class S>
const Serializer &operator<<(Serializer &ser, const std::pair<F, S> &v) {
    ser << v.first << v.second;
    return ser;
}

template <class F, class S>
const Serializer &operator>>(Serializer &ser, const std::pair<F, S> &v) {
    ser >> v.first >> v.second;
    return ser;
}

template <class K, class V>
const Serializer &operator<<(Serializer &ser, const std::map<K, V> &v) {
    ser << v.size();
    for (auto &i : v) {
        ser << i;
    }
    return ser;
}

template <class K, class V>
const Serializer &operator>>(Serializer &ser, std::map<K, V> &v) {
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
