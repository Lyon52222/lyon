#ifndef __LYON_RPC_PROTOCOL_H__
#define __LYON_RPC_PROTOCOL_H__
#include "lyon/serialize/serializer.h"
#include <cstdint>
#include <memory>
namespace lyon::rpc {

/**
 * @brief 通信协议
 * +------+
 * | 1byte | 1byte   | 1byte | 1tyte | 4byte  |  4byte      |
 * +------+
 * | magic | version | type  | flag  | seq_id | content_len | content |
 * +------+
 */
class RPCProtocol {
public:
    typedef std::shared_ptr<RPCProtocol> ptr;

    static constexpr uint8_t MAGIC = 0x34;
    //当前协议版本号
    static constexpr uint8_t VERSION = 0x10;
    //协议头长度
    static constexpr uint8_t HEAD_LEN = 0x0C;
    enum class MSG_TYPE : uint8_t {
        //心跳包
        RPC_HEART_BEAT = 0,

        RPC_PROVIDER,
        RPC_CONSUMER,

        //向注册中心注册方法
        RPC_REGIST_REQUEST,
        RPC_REGIST_RESPONSE,

        //向注册中心请求
        RPC_METHOD_REQUEST,
        RPC_METHOD_RESPONSE
    };

private:
    //魔法数字
    uint8_t m_magic = MAGIC;
    //当前协议版本
    uint8_t m_version = VERSION;
    //数据类型
    uint8_t m_type = 0;
    //标记位
    // 0 0 0 0 0 0 |        0       |       0
    //             | 数据包是否紧急 |数据是否使用varint压缩
    uint8_t m_flag = 0x01;
    //数据包编号
    uint32_t m_seqId = 0;
    //内容长度
    uint32_t m_contentLen = 0;
    //数据包内容
    Serializer::ptr m_ser;
};

} // namespace lyon::rpc

#endif
