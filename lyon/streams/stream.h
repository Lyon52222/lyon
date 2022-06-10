#ifndef __LYON_STREAMS_STREAM_H__
#define __LYON_STREAMS_STREAM_H__

#include "lyon/serialize/bytearray.h"
#include <memory>
namespace lyon {

/**
 * @brief 数据流的虚基类，封装了读写操作
 */
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
    virtual int write(const void *buffer, size_t size) = 0;

    virtual int write(ByteArray::ptr bytearray, size_t size) = 0;

    //因为，read
    // write可能无法保证能够一次写完或读取所有内容。所以，创建FixSize来保证

    int readFixSize(void *buffer, size_t size);
    int readFixSize(ByteArray::ptr bytearray, size_t size);

    int writeFixSize(const void *buffer, size_t size);
    int writeFixSize(ByteArray::ptr bytearray, size_t size);

    virtual void close() = 0;
    virtual ~Stream(){};
};

} // namespace lyon

#endif
