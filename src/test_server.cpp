// 2022/5/23
// 测试RPC服务器的类

#include <iostream>
#include <string>
#include <unordered_map>
#include <sys/sysinfo.h>

#include "../include/minico/logger.h"
#include "../include/rpc/rpc_server.h"

// 注册的一种Service
class HelloWorld : public Service
{
public:
	typedef void (HelloWorld::*Func)(TinyJson &request, TinyJson &result);

	HelloWorld() : _name("HelloWorld")
	{
		_methods["hello"] = &HelloWorld::hello;
		_methods["world"] = &HelloWorld::world;
	}
	virtual const char *name() const
	{
		return _name.c_str();
	}
	virtual ~HelloWorld() {}

	// 实际的服务类的处理函数
	virtual void process(TinyJson &request, TinyJson &result) override
	{
		std::string method = request.Get<std::string>("method");
		LOG_INFO("the request method is %s", method.c_str());
		if (method.empty())
		{
			result["err"].Set(400);
			result["errmsg"].Set("request has no method");
			return;
		}
		auto it = _methods.find(method);
		if (it == _methods.end())
		{
			result["err"].Set(404);
			result["errmsg"].Set("method not found");
			return;
		}
		// 找到服务的对应接口 那么执行即可
		(this->*(it->second))(request, result);
	}
	// 需要用户重载的实际逻辑部分
	virtual void hello(TinyJson &request, TinyJson &result) = 0;
	virtual void world(TinyJson &request, TinyJson &result) = 0;

private:
	std::unordered_map<std::string, Func> _methods;
	std::string _name;
};

class HelloWorldImpl : public HelloWorld
{
public:
	HelloWorldImpl() = default;
	virtual ~HelloWorldImpl() = default;

	virtual void hello(TinyJson &request, TinyJson &result)
	{
		result["method"].Set("hello");
		result["err"].Set(200);
		result["errmsg"].Set("the loop problem is solved");
	}

	virtual void world(TinyJson &request, TinyJson &result)
	{
		result["method"].Set("world");
		result["err"].Set(200);
		result["errmsg"].Set("ok");
	}
};

int main()
{
	LOG_INFO("test: start the server");

	RpcServer rpc_server;
	rpc_server.add_service(new HelloWorldImpl);
	rpc_server.start_multi(nullptr, 7629);

	minico::sche_join();
	return 0;
}
