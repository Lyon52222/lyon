#include "bytearray.h"
#include "lyon/log.h"
#include "lyon/utils/endian.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <memory>
#include <stdexcept>
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

void ByteArray::reset() {
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

void ByteArray::clear() {
    m_position = m_size = 0;
    m_cur = m_head;
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

bool ByteArray::setPosition(size_t position) {
    if (position > m_capacity) {
        return false;
    }
    m_position = position;
    if (m_position > m_size) {
        m_size = m_position;
    }
    m_cur = m_head;
    size_t n = m_position / m_capacity;
    while (n--) {
        m_cur = m_cur->next;
    }
    return true;
}

void ByteArray::getReadBuffer(std::vector<iovec> &buffers, size_t size) {
    if (size > getReadableSize()) {
        throw std::out_of_range("ByteArray::getReadBuffer out of range");
    }
    size_t npos = m_position % m_baseSize;
    size_t ncap = m_cur->size - npos;
    Node *cur = m_cur;
    while (size > 0) {
        iovec buffer;
        memset(&buffer, 0, sizeof(buffer));
        if (size <= ncap) {
            buffer.iov_base = cur->ptr + npos;
            buffer.iov_len = size;
            //这里只是将自己的内存空间借出去，具体用了多少需要调用setPosition来设置。
            // m_position += size;
            size = 0;
        } else {
            buffer.iov_base = cur->ptr + npos;
            buffer.iov_len = ncap;
            // m_position += ncap;
            size -= ncap;
            cur = cur->next;
            ncap = cur->size;
            npos = 0;
        }
        buffers.push_back(buffer);
    }
}

void ByteArray::getWriteBuffer(std::vector<iovec> &buffers, size_t size) {
    if (size == 0) {
        return;
    }
    testCapacity(size);
    size_t npos = m_position % m_baseSize;
    size_t ncap = m_cur->size - npos;
    Node *cur = m_cur;
    while (size > 0) {
        iovec buffer;
        memset(&buffer, 0, sizeof(buffer));
        if (size <= ncap) {
            buffer.iov_base = cur->ptr + npos;
            buffer.iov_len = size;
            // m_position += size;
            size = 0;
        } else {
            buffer.iov_base = cur->ptr + npos;
            buffer.iov_len = ncap;
            // m_position += ncap;
            size -= ncap;
            cur = cur->next;
            ncap = cur->size;
            npos = 0;
        }
        buffers.push_back(buffer);
    }
}

void ByteArray::getReadBuffer(std::vector<iovec> &buffers, size_t size,
                              size_t position) {

    if (size > (m_size - position)) {
        throw std::out_of_range("ByteArray::getReadBuffer out of range");
    }
    size_t npos = position % m_baseSize;
    Node *cur = m_cur;
    size_t count = position / m_baseSize;
    while (count > 0) {
        cur = cur->next;
        --count;
    }
    size_t ncap = cur->size - npos;
    while (size > 0) {
        iovec buffer;
        memset(&buffer, 0, sizeof(buffer));
        if (size <= ncap) {
            buffer.iov_base = cur->ptr + npos;
            buffer.iov_len = size;
            size = 0;
        } else {
            buffer.iov_base = cur->ptr + npos;
            buffer.iov_len = ncap;
            size -= ncap;
            cur = cur->next;
            ncap = cur->size;
            npos = 0;
        }
        buffers.push_back(buffer);
    }
}

std::string ByteArray::toString() {
    size_t len = getReadableSize();
    std::string str;
    if (len <= 0) {
        return str;
    }
    str.resize(len);
    read(&str[0], len);
    return str;
}

std::string ByteArray::toString(size_t position) {
    size_t len = m_size - position;
    std::string str;
    if (len <= 0) {
        return str;
    }
    str.resize(len);
    read(&str[0], len, position);
    return str;
}

void ByteArray::write(const void *buf, size_t size) {
    if (size == 0) {
        return;
    }
    testCapacity(size);
    //当前数据块已使用容量
    size_t npos = m_position % m_baseSize;
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

void ByteArray::read(const void *buf, size_t size) {
    if (size > getReadableSize()) {
        throw std::out_of_range("ByteArray::read out of range");
    }
    size_t npos = m_position % m_baseSize;
    size_t ncap = m_cur->size - npos;
    size_t bpos = 0;
    while (size > 0) {
        if (size <= ncap) {
            memcpy((char *)buf + bpos, m_cur->ptr + npos, size);
            if (size == ncap) {
                m_cur = m_cur->next;
            }
            m_position += size;
            size = 0;
        } else {
            memcpy((char *)buf + bpos, m_cur->ptr + npos, ncap);
            m_position += ncap;
            bpos += ncap;
            size -= ncap;
            m_cur = m_cur->next;
            ncap = m_cur->size;
            npos = 0;
        }
    }
}

void ByteArray::read(const void *buf, size_t size, size_t position) {
    if (position < 0 || size > (m_size - position)) {
        throw std::out_of_range("ByteArray::read(position) out of range");
    }
    size_t npos = position % m_baseSize;
    size_t ncap = m_cur->size - npos;
    size_t bpos = 0;
    while (size > 0) {
        if (size <= ncap) {
            memcpy((char *)buf + bpos, m_cur->ptr + npos, size);
            if (size == ncap) {
                m_cur = m_cur->next;
            }
            size = 0;
        } else {
            memcpy((char *)buf + bpos, m_cur->ptr + npos, ncap);
            bpos += ncap;
            size -= ncap;
            m_cur = m_cur->next;
            ncap = m_cur->size;
            npos = 0;
        }
    }
}

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

void ByteArray::writeFloat(float value) {
    uint32_t t = 0;
    memcpy(&t, &value, sizeof(float));
    writeFuint32(t);
}

void ByteArray::writeDouble(double value) {
    uint64_t t = 0;
    memcpy(&t, &value, sizeof(double));
    writeFint64(t);
}

void ByteArray::writeStringF16(const std::string &str) {
    writeFint16(str.size());
    write(str.c_str(), str.size());
}

void ByteArray::writeStringF32(const std::string &str) {
    writeFint32(str.size());
    write(str.c_str(), str.size());
}

void ByteArray::writeStringF64(const std::string &str) {
    writeFint64(str.size());
    write(str.c_str(), str.size());
}

void ByteArray::writeStringVarint(const std::string &str) {
    writeUint64(str.size());
    write(str.c_str(), str.size());
}

void ByteArray::writeStringRaw(const std::string &str) {
    write(str.c_str(), str.size());
}

bool ByteArray::readFromFile(const std::string &file) {
    std::ifstream ifs;
    ifs.open(file, std::ios::binary);
    if (!ifs) {
        LYON_LOG_ERROR(g_logger)
            << "ByteArray::readFromFile error file = " << file
            << " error = " << strerror(errno);
        return false;
    }
    std::shared_ptr<char> buff(new char[m_baseSize],
                               [](char *ptr) { delete[] ptr; });
    while (!ifs.eof()) {
        ifs.read(buff.get(), m_baseSize);
        write(buff.get(), ifs.gcount());
    }
    return true;
}

void ByteArray::loadFromStr(const std::string &content) {
    clear();
    write(content.c_str(), content.size());
}

int8_t ByteArray::readFint8() {
    int8_t val = 0;
    read(&val, sizeof(int8_t));
    return val;
}

uint8_t ByteArray::readFuint8() {
    uint8_t val = 0;
    read(&val, sizeof(uint8_t));
    return val;
}

#define XX(type)                                                               \
    type val = 0;                                                              \
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
    uint32_t result = 0;
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
    uint64_t result = 0;
    for (int i = 0; i < 64; i += 7) {
        uint8_t t = readFuint8();
        if (t < 0x80) {
            result |= ((uint64_t)t) << i;
            break;
        } else {
            result |= ((uint64_t)t & 0x7F) << i;
        }
    }
    return result;
}

float ByteArray::readFloat() {
    float tf = 0;
    uint32_t ti = readFuint32();
    memcpy(&tf, &ti, sizeof(uint32_t));
    return tf;
}

double ByteArray::readDouble() {
    double td = 0;
    uint64_t ti = readFuint64();
    memcpy(&td, &ti, sizeof(uint64_t));
    return td;
}

std::string ByteArray::readStringF16() {
    std::string str;
    uint16_t size = readFuint16();
    str.resize(size);
    read(&str[0], size);
    return str;
}

std::string ByteArray::readStringF32() {
    std::string str;
    uint32_t size = readFuint32();
    str.resize(size);
    read(&str[0], size);
    return str;
}

std::string ByteArray::readStringF64() {
    std::string str;
    uint64_t size = readFuint64();
    str.resize(size);
    read(&str[0], size);
    return str;
}

std::string ByteArray::readStringVarint() {
    std::string str;
    uint64_t size = readUint64();
    str.resize(size);
    read(&str[0], size);
    return str;
}

bool ByteArray::writeToFile(const std::string &file) const {
    std::ofstream ofs;
    ofs.open(file, std::ios::trunc | std::ios::binary);
    if (!ofs) {
        LYON_LOG_ERROR(g_logger)
            << "ByteArray::writeToFile error file = " << file
            << " error = " << strerror(errno);
        return false;
    }
    size_t size = getReadableSize();
    size_t pos = m_position;
    Node *cur = m_cur;
    while (size) {
        size_t gap = pos % m_baseSize;
        size_t len = (size > m_baseSize ? m_baseSize : size) - gap;
        ofs.write(cur->ptr + gap, len);
        cur = cur->next;
        pos += len;
        size -= len;
    }
    return true;
}

} // namespace lyon
