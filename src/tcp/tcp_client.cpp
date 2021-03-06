// 2022/5/23
// 基于协程的客户端，一个客户端只用来保存一个连接，客户端必须在一个协程中运行

#include "../../include/tcp/tcp_client.h"

// 客户端的核心连接函数，需要用一个协程去连接，防止本函数阻塞
void TcpClient::connect(const char *ip, int port)
{
    // 调用client_socket的连接函数
    LOG_INFO("the client connection to the server");
    return m_client_socket->connect(ip, port);
}

int TcpClient::disconnect()
{
    // 关闭写，如果读到了0就关掉
    return m_client_socket->shutdownWrite();
}

size_t TcpClient::recv(void *buf, size_t count)
{
    return m_client_socket->read(buf, count);
}

size_t TcpClient::send(const void *buf, size_t count)
{
    return m_client_socket->send(buf, count);
}
