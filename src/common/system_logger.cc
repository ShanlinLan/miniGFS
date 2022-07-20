#include "src/common/system_logger.h"

namespace gfs
{
    namespace common
    {
        void SystemLogger::Initialize(const std::string &program_name, const bool use_failure_signal_handler)
        {
            if (this->is_initialized_)
            {
                return;
            }

            FLAGS_logtostderr = true;

            google::InitGoogleLogging(program_name.c_str());

            if (use_failure_signal_handler)
            {
                google::InstallFailureSignalHandler();
                this->is_initialized_ = true;
            }
        }
    }
}