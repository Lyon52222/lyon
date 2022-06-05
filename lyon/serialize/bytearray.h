#ifndef __LYON_BYTEARRAY_H__
#define __LYON_BYTEARRAY_H__

#include <cstddef>
#include <cstdint>
#include <memory>
#include <sys/uio.h>
#include <vector>

namespace lyon {

class ByteArray {
public:
    typedef std::shared_ptr<ByteArray> ptr;

    struct Node {
        Node(size_t s);
        Node();
        ~Node();

        size_t size;
        char *ptr;
        Node *next;
    };

    ByteArray(size_t base_size = 4096);
    ~ByteArray();

    /**
     * @brief 重置状态到初始化状态(只有一个Node): 释放掉多余空间
     *
     */
    void reset();

    /**
     * @brief 清空已经用空间：并不释放空间
     *
     */
    void clear();

    /**
     * @brief 获得当前可读取的数据大小
     *
     * @return 可读取的数据大小
     */
    size_t getReadableSize() const { return m_size - m_position; }

    /**
     * @brief 获取剩余容量
     *
     * @return 剩余容量大小
     */
    size_t getRemaindCapacity() const { return m_capacity - m_position; }

    /**
     * @brief 增加容量大小
     *
     * @param size 需要增加的大小
     * @return 增加容量后的容量大小
     */
    size_t addCapacity(size_t size);

    /**
     * @brief 测试容量大小，如果剩余容量大于等于size则直接返回。否则进行扩展
     *
     * @param size 需求容量大小
     * @return 剩余容量大小
     */
    size_t testCapacity(size_t size);

    size_t getPosition() const { return m_position; }
    bool setPosition(size_t position);

    void getReadBuffer(std::vector<iovec> &buffers, size_t size);
    void getReadBuffer(std::vector<iovec> &buffers, size_t size,
                       size_t position);
    /**
     * @brief 获取待写入的内存空间
     *
     * @param buffers 内存空间数组
     * @param size 需要的内存大小
     */
    void getWriteBuffer(std::vector<iovec> &buffers, size_t size);

    /**
     * @brief 将可读取区域的内容转化为字符串
     *
     */
    std::string toString();

    /**
     * @brief 从position开始将缓存中的内容转化为字符串
     *
     * @param position 读取开始的位置
     */
    std::string toString(size_t position);

    void write(const void *buf, size_t size);
    void read(const void *buf, size_t size);
    void read(const void *buf, size_t size, size_t position);

    //以固定长度写入
    void writeFint8(int8_t value);
    void writeFuint8(uint8_t value);
    void writeFint16(int16_t value);
    void writeFuint16(uint16_t value);
    void writeFint32(int32_t value);
    void writeFuint32(uint32_t value);
    void writeFint64(int64_t value);
    void writeFuint64(uint64_t value);

    //以Varint格式写入
    void writeInt32(int32_t value);
    void writeUint32(uint32_t value);
    void writeInt64(int64_t value);
    void writeUint64(uint64_t value);

    void writeFloat(float value);
    void writeDouble(double value);
    void writeStringF16(const std::string &str);
    void writeStringF32(const std::string &str);
    void writeStringF64(const std::string &str);
    void writeStringVarint(const std::string &str);

    bool readFromFile(const std::string &file);

    int8_t readFint8();
    uint8_t readFuint8();
    int16_t readFint16();
    uint16_t readFuint16();
    int32_t readFint32();
    uint32_t readFuint32();
    int64_t readFint64();
    uint64_t readFuint64();

    int32_t readInt32();
    uint32_t readUint32();
    int64_t readInt64();
    uint64_t readUint64();

    float readFloat();
    double readDouble();
    std::string readStringF16();
    std::string readStringF32();
    std::string readStringF64();
    std::string readStringVarint();

    bool writeToFile(const std::string &file) const;

private:
    /**
     * @m_size 已经使用的空间大小
     */
    size_t m_size;
    /**
     * @m_capacity 最大容量
     */
    size_t m_capacity;
    /**
     * @m_position 当前操作的位置,这个位置是可以被重置的
     */
    size_t m_position;
    /**
     * @m_baseSize 每个内存块的大小
     */
    size_t m_baseSize;
    /**
     * @m_endian 数据用那种字节序
     */
    int8_t m_endian;

    /**
     * @{name} 数据块链表头节点
     */
    Node *m_head;
    /**
     * @{name} 当前数据库节点指针
     */
    Node *m_cur;

    Node *m_tail;
};

} // namespace lyon
#endif // !__LYON_BYTEARRAY_H__
