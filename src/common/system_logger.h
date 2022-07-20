#ifndef GFS_COMMON_SYSTEM_LOGGER_H_
#define GFS_COMMON_SYSTEM_LOGGER_H_

#include <glog/logging.h>
#include <glog/stl_logging.h>
#include <string>

namespace gfs
{
    namespace common
    {

        class SystemLogger
        {
        public:
            static SystemLogger &GetInstance()
            {
                static SystemLogger instance;
                return instance;
            }

            SystemLogger(const SystemLogger &) = delete;

            void operator=(const SystemLogger &) = delete;

            void Initialize(const std::string &program_name, const bool use_failure_signal_handler = false);

        private:
            bool is_initialized_;

            SystemLogger() : is_initialized_(false){};
        };
    }
}

#define ASSERT_EQ(val1, val2) CHECK_EQ(val1, val2)
#define ASSERT_NE(val1, val2) CHECK_NE(val1, val2)
#define ASSERT_LE(val1, val2) CHECK_LE(val1, val2)
#define ASSERT_LT(val1, val2) CHECK_LT(val1, val2)
#define ASSERT_GE(val1, val2) CHECK_GE(val1, val2)
#define ASSERT_GT(val1, val2) CHECK_GT(val1, val2)

#define ASSERT_NOTNULL(val) CHECK_NOTNULL(val)
#define ASSERT_ERR(invocation) CHECK_ERR(invocation)

#endif