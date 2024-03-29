# Lyon
## 介绍：
该项目为本人学习C++后端的实践项目。
项目使用C++20开发，并再其中尝试使用Modern-Cpp

## Requirement:
yaml-cpp
ragel
hiredis


## 模块

### 日志模块
- 使用配置文件配置logger

### 配置模块
- 使用yaml作为配置文件，使用yaml-cpp进行解析。
- 配置使用约定优先，即在代码中规定好配置项。如果配置文件中给出了新的配置，使用新配置。

### 协程模块
- 使用ucontext实现的有栈协程。
- 协程为非对称协程，即只能由主协程切换到子协程，不能子协程切换到子协程。
- 主协程运行for循环遍历任务队列，如果有任务切换到子协程运行任务。
- 没有任务时候运行idle函数。
    - idle函数中进行epoll和定时器的检测。

### HTTP模块
- 对Socket，Addr等进行封装。
- 使用ragel编写状态机来解析HTTP请求和响应。

### RPC模块
使用返回值类型，参数类型，函数名来标识一个调用方法。因此框架可以支持函数重载。
#### 注册中心
- 负责管理注册方法。
- 服务端上线时需要在注册中心注册自己所提供的RPC调用。
- 客户端在请求方法时需要向注册中心查询提供该调用的服务端列表。

#### 服务端
- 服务端负责提供方法调用。
- 上线时注册方法，下线时取消注册，收到心跳包请求时返回心跳包。
#### 客户端
- 提供三种异步调用方式。
    - 直接调用：通过hook实现异步。
    - 通过callback函数实现。
    - 通过promise，feture实现。
- 维护一个服务端队列，每个方法对应了一个服务端队列。每次方法请求先尝试从队列中的服务器请求。
    - 当请求失败时向注册中心反应，通知注册中心发送心跳检测包，检测服务端是否在线。
    - 当服务端队列全部失效时向注册中心请求提供该方法的服务端列表。

## 未开发内容
- 消息队列：因为RPC返回和请求的顺序可能不同，因此需要一个消息队列来接收和分发返回。
- 将注册中心的存储队列改用Redis。
- 协程锁。

## 参考项目
- [sylar](https://github.com/sylar-yin/sylar)
- [rest_rpc](https://github.com/qicosmos/rest_rpc)
- [acid](https://github.com/zavier-wong/acid)
- [tinyrpc](https://github.com/Gooddbird/tinyrpc)
