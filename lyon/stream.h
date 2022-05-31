#ifndef __LYON__STREAM_H__
#define __LYON__STREAM_H__

#include "bytearray.h"
#include <memory>
namespace lyon {

class Stream {
public:
    typedef std::shared_ptr<Stream> ptr;

    /**
     * @brief 从流中读取数据到buffer中
     *
     * @param buffer 存放数据的内存空间
     * @param size 需要读取数据的长度
     * @return 读取的字节数
     */
    virtual int read(void *buffer, size_t size) = 0;

    virtual int read(ByteArray::ptr bytearray, size_t size) = 0;

    /**
     * @brief 将buffer中的数据写入到流中
     *
     * @param buffer 存放数据的内存空间
     * @param lenght 数据长度
     * @return 写入的子节数
     */
    virtual int write(void *buffer, size_t size) = 0;

    virtual int write(ByteArray::ptr bytearray, size_t size) = 0;
};

} // namespace lyon

#endif
