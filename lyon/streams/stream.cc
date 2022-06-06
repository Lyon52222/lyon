#include "stream.h"
#include <iostream>
namespace lyon {

int Stream::readFixSize(void *buffer, size_t size) {
    size_t remaind = size;
    while (remaind > 0) {
        size_t readed = read(buffer, remaind);
        if (readed <= 0) {
            return readed;
        }
        remaind -= readed;
    }
    return size;
}

int Stream::readFixSize(ByteArray::ptr bytearray, size_t size) {
    size_t remaind = size;
    while (remaind > 0) {
        // std::cout << remaind << std::endl;
        size_t readed = read(bytearray, remaind);
        if (readed <= 0) {
            return readed;
        }
        remaind -= readed;
    }
    return size;
}

int Stream::writeFixSize(const void *buffer, size_t size) {
    size_t remaind = size;
    while (remaind > 0) {
        size_t writed = write(buffer, remaind);
        if (writed <= 0) {
            return writed;
        }
        remaind -= writed;
    }
    return size;
}

int Stream::writeFixSize(ByteArray::ptr bytearray, size_t size) {
    size_t remaind = size;
    while (remaind > 0) {
        size_t writed = write(bytearray, remaind);
        if (writed <= 0) {
            return writed;
        }
        remaind -= writed;
    }
    return size;
}

} // namespace lyon
