// 2022/5/23
// 测试客户端的程序

#include <iostream>
#include <string>
#include <sys/sysinfo.h>

#include "../include/minico/logger.h"
#include "../include/rpc/rpc_client.h"

// 问题初步分析是由于rpc客户端销毁造成一直发送0造成的
void tcp_client_worker(TcpClient &tcp_client)
{
    tcp_client.connect("127.0.0.1", 12345);
    char buf[1024];

    LOG_INFO("client send ping");
    tcp_client.send("ping", 4);
    tcp_client.recv(buf, 1024);
    LOG_INFO("client recv %s", buf);
}

void rpc_client_worker(RpcClient &rpc_client, int number)
{
    rpc_client.connect("162.14.64.62", 7629);
    for (int i = 0; i < number; ++i)
    {
        LOG_INFO("-------the %d st client test-----------", i);
        rpc_client.ping();
        TinyJson request;
        TinyJson result;

        request["service"].Set<std::string>("HelloWorld");
        request["method"].Set<std::string>("world");
        rpc_client.call(request, result);
        int errcode = result.Get<int>("err");
        std::string errmsg = result.Get<std::string>("errmsg");
        LOG_INFO("--------------------------------");
        LOG_INFO("the result errcode is %d", errcode);
        LOG_INFO("the result errmsg is %s", errmsg.c_str());
        LOG_INFO("--------------------------------");

        TinyJson request_two;
        TinyJson result_two;
        request_two["service"].Set<std::string>("HelloWorld");
        request_two["method"].Set<std::string>("hello");
        rpc_client.call(request_two, result_two);
        int errcode_two = result_two.Get<int>("err");
        std::string errmsg_two = result_two.Get<std::string>("errmsg");
        LOG_INFO("--------------------------------");
        LOG_INFO("the result errcode is %d", errcode_two);
        LOG_INFO("the result errmsg is %s", errmsg_two.c_str());
        LOG_INFO("--------------------------------");
    }
}

int main()
{
    LOG_INFO("test: add one rpc client");
    RpcClient rpc_client_test;
    int loop_time = 10;
    minico::co_go([&rpc_client_test, &loop_time]()
                  { rpc_client_worker(rpc_client_test, loop_time); });
    minico::sche_join();
    return 0;
}