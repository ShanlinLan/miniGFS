// 2022/5/27
// GFS客户端

#include "gfs_client.h"

namespace gfs
{
    namespace client
    {
        bool ValidateOpenFlag(unsigned int flags)
        {
            return flags == OpenFlag::Read || flags == OpenFlag::Write ||
                   flags == OpenFlag::Create ||
                   flags == (OpenFlag::Write | OpenFlag::Create);
        }

        
    };
};