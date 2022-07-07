#ifndef GFS_COMMON_UTILS_H_
#define GFS_COMMON_UTILS_H_

// 2022/5/27
// GFS的工具类

#include "yaml-cpp/yaml.h"

namespace gfs
{
    enum StatusCode
    {
        OK = 0,
        ALREADY_EXISTS,
        InvalidArgumentError
    };

    namespace common
    {
        namespace utils
        {
            StatusCode ValidateConfigFile(const YAML::Node &node);
        };
    };
};

#endif