#include "stream.h"
#include <iostream>
#include <lyon/log.h>
namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

ssize_t Stream::readFixSize(void *buffer, size_t size) {
    size_t remaind = size;
    while (remaind > 0) {
        //这里千万不要用size_t，因为size_t对应的是usigined int无法正确接收-1
        ssize_t readed = read(buffer, remaind);
        if (readed <= 0) {
            return readed;
        }
        remaind -= readed;
    }
    return size;
}

ssize_t Stream::readFixSize(ByteArray::ptr bytearray, size_t size) {
    size_t remaind = size;
    while (remaind > 0) {
        ssize_t readed = read(bytearray, remaind);
        if (readed <= 0) {
            return readed;
        }
        remaind -= readed;
    }
    return size;
}

ssize_t Stream::writeFixSize(const void *buffer, size_t size) {
    size_t remaind = size;
    while (remaind > 0) {
        ssize_t writed = write(buffer, remaind);
        if (writed <= 0) {
            return writed;
        }
        remaind -= writed;
    }
    return size;
}

ssize_t Stream::writeFixSize(ByteArray::ptr bytearray, size_t size) {
    size_t remaind = size;
    while (remaind > 0) {
        ssize_t writed = write(bytearray, remaind);
        if (writed <= 0) {
            return writed;
        }
        remaind -= writed;
    }
    return size;
}

} // namespace lyon
