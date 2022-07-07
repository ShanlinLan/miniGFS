#pragma once

// 2022/5/23
// 基于协程的tcp服务器，一个连接对应一个协程

#include <functional>
#include <memory>
#include <sys/sysinfo.h>

#include "../minico/logger.h"
#include "../minico/socket.h"
#include "../minico/minico_api.h"

// 该类是一个基类，可以被支持不同业务的服务器复用
class TcpServer
{
public:
    // 用户注册的服务器的连接回调函数，这里需要注意connection的生命期
    typedef std::function<void(minico::Socket *)> conn_callback;

    TcpServer() : _listen_fd(nullptr), _multi_listen_fd(nullptr),
                  server_ip(nullptr), server_port(-1)
    {
        LOG_INFO("constructor the tcp server finish");
    }

    virtual ~TcpServer()
    {
        delete _listen_fd;
        _listen_fd = nullptr;
        delete[] _multi_listen_fd;
        _multi_listen_fd = nullptr;
        delete server_ip;
        server_ip = nullptr;
    }

    // 单线程模式下开启服务器的运行 
    void start(const char *ip, int port);

    // 多线程模式下开始服务器的运行 
    void start_multi(const char *ip, int port);

    // 注册连接函数的接口，这里选择使用一层转发 
    inline void register_connection(const conn_callback &func)
    {
        this->register_connection(conn_callback(func));
    }

    inline void register_connection(conn_callback &&func)
    {
        _on_server_connection.reset(new conn_callback(std::move(func)));
    }

private:
    // 服务器的loop工作函数 
    void server_loop();

    // 服务器多核loop函数 
    void multi_server_loop(int thread_number);

    // 保存客户端连接时触发的回调函数，这里不要保存tcp连接 
    std::shared_ptr<conn_callback> _on_server_connection;

    // 用于监听的socket 
    minico::Socket *_listen_fd;

    // 多线程下监听的socket队列 
    minico::Socket *_multi_listen_fd;

    const char *server_ip;  // 服务器IP地址
    int server_port;        // 服务器监听端口
};
