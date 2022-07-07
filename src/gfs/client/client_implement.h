#ifndef GFS_CLIENT_CLIENT_IMPLEMENT_H
#define GFS_CLIENT_CLIENT_IMPLEMENT_H

// 2022/5/27
//

#include <cstddef>
#include <string>

#include "../common/utils.h"
#include "../common/config_manager.h"

namespace gfs
{
    namespace client
    {
        // 读操作返回的数据类型
        struct Data
        {
            size_t bytes_read; // 读取的数据大小
            void *buffer;      // 数据缓冲区的指针
            Data() : bytes_read(0), buffer(nullptr) {}
            Data(size_t _bytes_read, void *_buffer)
                : bytes_read(_bytes_read), buffer(_buffer) {}
        };

        class ClientImplement
        {
        public:
            StatusCode CreateFile(const std::string &filename);
            Data ReadFile(const char *filename, size_t offset, size_t nbytes);
            StatusCode WriteFile(const char *filename, void *buffer,
                                 size_t offset, size_t nbytes);
            StatusCode DeleteFile(const std::string &filename);
            static ClientImplement *ConstructClientImplement(
                const std::string &config_filename, const std::string &master_name,
                const bool resolve_hostname = false);

        private:
            ClientImplement() = default;
            ClientImplement(common::ConfigManager *config_manager,
                            const std::string &config_name, const bool resolve_hostname = false);
        };

    };
};

#endif
