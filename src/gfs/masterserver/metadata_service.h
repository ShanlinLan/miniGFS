// 2022/5/23
// master server提供的rpc服务

#include <iostream>
#include <string>
#include <unordered_map>
#include <sys/sysinfo.h>

#include "../minico/json.h"
#include "../minico/logger.h"
#include "../rpc/service.h"
#include "metadata_manager.h"

namespace masterserver
{
    class MetadataService : public Service
    {
    public:
        typedef void (MetadataService::*Func)(TinyJson &request, TinyJson &result);

        MetadataService() : _name("MetadataService")
        {
            _methods["HandleFileCreation"] = &MetadataService::HandleFileCreation;
        }

        virtual const char *name() const
        {
            return _name.c_str();
        }

        virtual ~MetadataService() {}

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

        // 提供给client的写数据服务
        virtual void HandleFileCreation(TinyJson &request, TinyJson &result) = 0;

    private:
        std::unordered_map<std::string, Func> _methods;
        std::string _name;
    };

    class MetadataServiceImplement : public MetadataService
    {
    public:
        MetadataServiceImplement() = default;
        virtual ~MetadataServiceImplement() = default;

        void HandleFileCreation(TinyJson &request, TinyJson &result);
    
    private:
        MetadataManager* metadata_manager();
    };
}