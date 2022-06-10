#ifndef __LYON_RPC_RESULT_H__
#define __LYON_RPC_RESULT_H__
#include <cstdint>
#include <lyon/serialize/serializer.h>
#include <memory>

namespace lyon::rpc {

enum class RpcResultState : std::uint8_t {
    //正常返回
    OK = 0,
    //未连接到服务器
    NOT_CONNECT,
    // Socket连接被对方关闭 rt==0
    SEND_CLOSE_BY_PEER,
    // Socket接收错误 rt<0
    SEND_SOCKET_ERROR,
    //未找到函数
    METHOD_NOT_FOUND,
    //方法运行错误
    METHOD_RUN_ERROR,
    //参数解析错误
    ARGS_ERROR,
    RECV_BAD_RESPONSE
};

/**
 * @brief RPC请求后返回的具体内容
 *
 * @tparam T 返回值的类型
 */
template <typename T = std::string> class RpcResult {
public:
    typedef T type;
    typedef std::shared_ptr<RpcResult> ptr;

    RpcResult() {}

    RpcResult(RpcResultState state, const std::string &msg) : m_msg(msg) {
        m_state = static_cast<uint8_t>(state);
    }

    uint8_t getState() const { return m_state; }
    const std::string &getMsg() const { return m_msg; }
    const T &getVal() const { return m_val; }

    void setState(uint8_t val) { m_val = val; }
    void setMsg(const std::string &msg) { m_msg = msg; }
    void setVal(const T &val) { m_val = val; }

    /**
     * @brief 将其转化为stirng
     *
     */
    std::string toString() {
        Serializer ser;
        ser << m_state << m_msg;
        return ser.toString();
    }

    friend const Serializer &operator<<(const Serializer &ser,
                                        RpcResult<T> &ret) {
        ser << ret.m_state << ret.m_msg;
        if (ret.m_state !=
                static_cast<uint8_t>(RpcResultState::METHOD_NOT_FOUND) &&
            ret.m_state !=
                static_cast<uint8_t>(RpcResultState::METHOD_RUN_ERROR))
            ser << ret.m_val;
        return ser;
    }

    friend const Serializer &operator>>(const Serializer &ser,
                                        RpcResult<T> &ret) {
        ser >> ret.m_state >> ret.m_msg;
        //这里的状态表示返回值信息是无效的。
        if (ret.m_state !=
                static_cast<uint8_t>(RpcResultState::METHOD_NOT_FOUND) &&
            ret.m_state !=
                static_cast<uint8_t>(RpcResultState::METHOD_RUN_ERROR))
            ser >> ret.m_val;
        return ser;
    }

private:
    uint8_t m_state = 0;
    std::string m_msg;
    T m_val;
};

} // namespace lyon::rpc

#endif
