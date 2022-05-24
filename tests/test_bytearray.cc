#include "lyon/bytearray.h"
#include <cstddef>
#include <cstdint>
#include <lyon/log.h>
#include <lyon/macro.h>
#include <stdlib.h>
#include <vector>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test_f() {
#define XX(type, len, write_fun, read_fun, base_size)                          \
    {                                                                          \
        std::vector<type> v;                                                   \
        for (size_t i = 0; i < len; i++)                                       \
            v.push_back(rand());                                               \
        lyon::ByteArray::ptr array(new lyon::ByteArray(base_size));            \
        for (auto &i : v) {                                                    \
            array->write_fun(i);                                               \
        }                                                                      \
        array->setPosition(0);                                                 \
        type t;                                                                \
        for (size_t i = 0; i < len; i++) {                                     \
            t = array->read_fun();                                             \
            LYON_ASSERT2(t == v[i], t << " " << v[i]);                         \
        }                                                                      \
        LYON_ASSERT(array->getReadableSize() == 0);                            \
        LYON_LOG_INFO(g_logger)                                                \
            << #type << " " << #write_fun << " " << #read_fun << " OK!";       \
    }

    XX(int8_t, 1, writeFint8, readFint8, 10);
    XX(uint8_t, 100, writeFuint8, readFuint8, 10);
    XX(int16_t, 100, writeFint16, readFint16, 10);
    XX(uint16_t, 100, writeFuint16, readFuint16, 10);
    XX(int32_t, 100, writeFint32, readFint32, 10);
    XX(uint32_t, 100, writeFuint32, readFuint32, 10);
    XX(int64_t, 100, writeFint64, readFint64, 10);
    XX(uint64_t, 100, writeFuint64, readFuint64, 10);

    XX(int32_t, 100, writeInt32, readInt32, 10);
    XX(uint32_t, 100, writeUint32, readUint32, 10);
    XX(int64_t, 100, writeInt64, readInt64, 10);
    XX(uint64_t, 100, writeUint64, readUint64, 10);
}

int main(int argc, char *argv[]) {
    test_f();
    return 0;
}
