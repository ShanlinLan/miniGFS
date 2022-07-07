#include "../../include/rpc/rpc_client.h"

void work(RpcClient& rpc_client)
{
    rpc_client.connect("162.14.64.62", 12345);
    TinyJson request;
    TinyJson result;
    request["service"].Set<std::string>("MetadataService");
    request["method"].Set<std::string>("HandleFileCreation");
    request["filename"].Set<std::string>("test.txt");
    rpc_client.call(request, result);
    int errcode = result.Get<int>("status");
    LOG_INFO("the result errcode is %d", errcode);
}

int main()
{
    LOG_INFO("test: add one rpc client");
    RpcClient rpc_client_test;
    minico::co_go([&rpc_client_test]()
                  { work(rpc_client_test); });
    minico::sche_join();
    return 0;
}