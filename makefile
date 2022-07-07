all: rpc_server rpc_client tcp_client tcp_server

rpc_client:
	g++ -std=c++11 -pthread -lpthread ./src/minico/*.cpp ./src/tcp/*.cpp ./src/rpc/*.cpp \
	./src/masterserver/metadata_manager.cpp ./src/masterserver/metadata_service.cpp ./src/client/client.cpp -o rpc_client

rpc_server:
	g++ -std=c++11 -pthread -lpthread ./src/minico/*.cpp ./src/tcp/*.cpp ./src/rpc/*.cpp ./src/masterserver/*.cpp -o rpc_server

tcp_client:
	g++ -std=c++11 -pthread -lpthread ./src/minico/*.cpp ./src/tcp/*.cpp ./src/test_tcp_client.cpp -o test_tcp_client
tcp_server:
	g++ -std=c++11 -pthread -lpthread ./src/minico/*.cpp ./src/tcp/*.cpp ./src/test_tcp_server.cpp -o test_tcp_server

.PHONY: clean
clean:
	rm -rf rpc_client rpc_server test_tcp_client test_tcp_server