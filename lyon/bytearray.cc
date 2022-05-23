#include "bytearray.h"
#include "log.h"
#include "utils/endian.h"
#include <cstdint>
#include <string.h>

namespace lyon {
static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

ByteArray::Node::Node(size_t s) : size(s), ptr(new char[s]), next(nullptr) {}

ByteArray::Node::Node() : size(0), ptr(nullptr), next(nullptr) {}

ByteArray::Node::~Node() {
    if (ptr)
        delete[] ptr;
}

ByteArray::ByteArray(size_t base_size)
    : m_size(0), m_capacity(base_size), m_position(0), m_baseSize(base_size),
      m_endian(LYON_BIG_ENDIAN), m_head(new Node(base_size)), m_cur(m_head),
      m_tail(m_head) {}

ByteArray::~ByteArray() {
    while (m_head) {
        Node *t = m_head;
        m_head = m_head->next;
        delete t;
    }
}

void ByteArray::clear() {
    Node *t = m_head->next;
    while (t) {
        m_cur = t;
        t = t->next;
        delete m_cur;
    }
    m_position = m_size = 0;
    m_capacity = m_baseSize;
    m_tail = m_cur = m_head;
}

size_t ByteArray::addCapacity(size_t size) {
    if (size <= 0) {
        return m_size;
    }
    bool is_empty = getRemaindCapacity() == 0;
    size_t n = ceil(1.0 * size / m_baseSize);
    Node *t;
    while (n--) {
        t = new Node(m_baseSize);
        m_tail->next = t;
        m_tail = t;
        m_capacity += m_baseSize;
        if (is_empty) {
            m_cur = t;
        }
    }

    return m_capacity;
}

size_t ByteArray::testCapacity(size_t size) {
    size_t remaindcapacity = getRemaindCapacity();
    if (remaindcapacity >= size) {
        return remaindcapacity;
    }
    addCapacity(size - remaindcapacity);
    return getRemaindCapacity();
}

void ByteArray::write(const void *buf, size_t size) {
    if (size == 0) {
        return;
    }
    testCapacity(size);
    //当前数据块已使用容量
    size_t npos = m_capacity % m_baseSize;
    //当前数据块剩余容量
    size_t ncap = m_cur->size - npos;
    // buf已读取容量
    size_t bpos = 0;
    while (size > 0) {
        //如果当前块能直接存放
        if (size <= ncap) {
            memcpy(m_cur->ptr + npos, (const char *)buf + bpos, size);
            //如果当前块刚好用完，将指针移动到下个块中。（感觉这里可以省略）
            if (size == ncap) {
                m_cur = m_cur->next;
            }
            m_position += size;
            size = 0;
        } else {
            memcpy(m_cur->ptr + npos, (const char *)buf + bpos, ncap);
            m_position += ncap;
            bpos += ncap;
            size -= ncap;
            m_cur = m_cur->next;
            ncap = m_cur->size;
            npos = 0;
        }
    }
    if (m_position > m_size) {
        m_size = m_position;
    }
}

void ByteArray::read(const void *buf, size_t size) {}

void ByteArray::writeFint8(int8_t value) { write(&value, sizeof(value)); }

void ByteArray::writeFuint8(uint8_t value) { write(&value, sizeof(value)); }

#define XX                                                                     \
    if (m_endian != LYON_BYTE_ORDER) {                                         \
        value = byteswap(value);                                               \
    }                                                                          \
    write(&value, sizeof(value));

void ByteArray::writeFint16(int16_t value) { XX }

void ByteArray::writeFuint16(uint16_t value) { XX }

void ByteArray::writeFint32(int32_t value) { XX }

void ByteArray::writeFuint32(uint32_t value) { XX }

void ByteArray::writeFint64(int64_t value) { XX }

void ByteArray::writeFuint64(uint64_t value) { XX }

#undef XX

//由于varint算法对负数的支持并不好，所以使用zigzag算法将有符号数转化为无符号数量。
static uint32_t Zigzag_encode_32(const int32_t &value) {
    return (uint32_t)((value << 1) ^ (value >> 31));
}

static uint64_t Zigzag_encode_64(const int64_t &value) {
    return (uint64_t)((value << 1) ^ (value >> 63));
}

static int32_t Zigzag_decode_32(const uint32_t &value) {
    return (int32_t)((value >> 1) ^ -(value & 1));
}

static int64_t Zigzag_decode_64(const uint64_t &value) {
    return (int64_t)((value >> 1) ^ -(value & 1));
}

void ByteArray::writeInt32(int32_t value) {
    writeUint32(Zigzag_encode_32(value));
}

//这里使用varint算法进行压缩。
void ByteArray::writeUint32(uint32_t value) {
    uint8_t tmp[5];
    uint8_t i = 0;
    while (value >= 0x80) {
        tmp[i++] = (value & 0x7F) | 0x80;
        value >>= 7;
    }
    tmp[i++] = value;
    write(tmp, i);
}

void ByteArray::writeInt64(int64_t value) {
    writeUint64(Zigzag_encode_64(value));
}

void ByteArray::writeUint64(uint64_t value) {
    uint8_t tmp[10];
    uint8_t i = 0;
    while (value >= 0x80) {
        tmp[i++] = (value & 0x7F) | 0x80;
        value >>= 7;
    }
    tmp[i++] = value;
    write(tmp, i);
}

int8_t ByteArray::readFint8() {
    int8_t val;
    read(&val, sizeof(int8_t));
    return val;
}

uint8_t ByteArray::readFuint8() {
    uint8_t val;
    read(&val, sizeof(uint8_t));
    return val;
}

#define XX(type)                                                               \
    type val;                                                                  \
    read(&val, sizeof(type));                                                  \
    if (m_endian == LYON_BYTE_ORDER)                                           \
        return val;                                                            \
    else                                                                       \
        return byteswap(val);

int16_t ByteArray::readFint16() { XX(int16_t); }
uint16_t ByteArray::readFuint16() { XX(uint16_t); }
int32_t ByteArray::readFint32() { XX(int32_t); }
uint32_t ByteArray::readFuint32() { XX(uint32_t); }
int64_t ByteArray::readFint64() { XX(int64_t); }
uint64_t ByteArray::readFuint64() { XX(uint64_t); }
#undef XX

int32_t ByteArray::readInt32() { return Zigzag_decode_32(readUint32()); }

uint32_t ByteArray::readUint32() {
    uint32_t result;
    for (int i = 0; i < 32; i += 7) {
        uint8_t t = readFuint8();
        if (t < 0x80) {
            result |= ((uint32_t)t) << i;
            break;
        } else {
            result |= ((uint32_t)t & 0x7F) << i;
        }
    }
    return result;
}

int64_t ByteArray::readInt64() { return Zigzag_decode_64(readUint64()); }

uint64_t ByteArray::readUint64() {
    uint64_t result;
    for (int i = 0; i < 64; i += 7) {
        uint8_t t = readFuint8();
        if (t < 0x80) {
            result |= ((uint64_t)t) << i;
            break;
        } else {
            result |= ((uint64_t)t) << i;
        }
    }
    return result;
}
} // namespace lyon
