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
        CANCELLED,
        INVALID_ARGUMENT,
        DEADLINE_EXCEEDED,
        NOT_FOUND,
        ALREADY_EXISTS,
        PERMISSION_DENIED,
        UNAUTHENTICATED,
        RESOURCE_EXHAUSTED,
        FAILED_PRECONDITION,
        ABORTED,
        OUT_OF_RANGE,
        UNIMPLEMENTED,
        INTERNAL,
        UNAVAILABLE,
        DATA_LOSS
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