#include "../../include/masterserver/metadata_service.h"
#include "../../include/rpc/rpc_server.h"

int main()
{
    LOG_INFO("test: start the server");

	RpcServer rpc_server;
	rpc_server.add_service(new masterserver::MetadataServiceImplement);
	rpc_server.start_multi(nullptr, 12345);

	minico::sche_join();
	std::cout << "end" << std::endl;
	return 0;
}