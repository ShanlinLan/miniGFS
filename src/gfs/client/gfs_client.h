#ifndef GFS_CLIENT_GFS_CLIENT_H_
#define GFS_CLIENT_GFS_CLIENT_H_

// 2022/5/27
// GFS客户端

#include <cstddef>
#include <string>

#include "../common/utils.h"
#include "client_implement.h"

namespace gfs
{

    // 定义open的操作标志
    enum OpenFlag
    {
        Read = 0x01,
        Write = 0x2,
        Create = 0x4
    };

    namespace client
    {
        StatusCode init_client(const std::string &config_filename,
                               const std::string &master_name, const bool resolve_hostname = false);

        void reset_client();

        StatusCode open(const char *filename, unsigned int flags);

        StatusCode close(const char *filename);

        Data read(const char *filename, size_t offset, size_t nbytes);

        StatusCode write(const char *path, void *buffer, size_t offset, size_t nbytes);

        StatusCode remove(const char *filename);

    }; // namespace client

}; // namespace gfs

#endif