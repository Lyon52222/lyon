#ifndef __LYON_BYTEARRAY_H__
#define __LYON_BYTEARRAY_H__

#include <cstddef>
#include <cstdint>
#include <memory>

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
    void clear();

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

    void write(const void *buf, size_t size);
    void read(const void *buf, size_t size);

    void writeFint8(int8_t value);
    void writeFuint8(uint8_t value);
    void writeFint16(int16_t value);
    void writeFuint16(uint16_t value);
    void writeFint32(int32_t value);
    void writeFuint32(uint32_t value);
    void writeFint64(int64_t value);
    void writeFuint64(uint64_t value);

    void writeInt32(int32_t value);
    void writeUint32(uint32_t value);
    void writeInt64(int64_t value);
    void writeUint64(uint64_t value);

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
     * @m_position 当前操作的位置
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
