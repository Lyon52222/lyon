#ifndef __LYON_UTILS_ENDIAN_H__
#define __LYON_UTILS_ENDIAN_H__
#include <type_traits>
#define LYON_LITTLE_ENDIAN 1
#define LYON_BIG_ENDIAN 2

#include <byteswap.h>
#include <stdint.h>
namespace lyon {

template <typename T>
typename std::enable_if<sizeof(T) == sizeof(uint16_t), T>::type
byteswap(T value) {
    return static_cast<T>(bswap_16(value));
}

template <typename T>
typename std::enable_if<sizeof(T) == sizeof(uint32_t), T>::type
byteswap(T value) {
    return static_cast<T>(bswap_32(value));
}

template <typename T>
typename std::enable_if<sizeof(T) == sizeof(uint64_t), T>::type
byteswap(T value) {
    return static_cast<T>(bswap_64(value));
}

#if BYTE_ORDER == BIG_ENDIAN
#define LYON_BYTE_ORDER LYON_BIG_ENDIAN
#else
#define LYON_BYTE_ORDER LYON_LITTLE_ENDIAN
#endif

#if (LYON_BYTE_ORDER == LYON_BIG_ENDIAN)
template <typename T> T byteswapOnBigEndian(T t) { return byteswap(t); }
template <typename T> T byteswapOnLittleEndian(T t) { return t; }
#else
template <typename T> T byteswapOnBigEndian(T t) { return t; }
template <typename T> T byteswapOnLittleEndian(T t) { return byteswap(t); }
#endif

} // namespace lyon
#endif
