#ifndef GFS_COMMON_CONFIG_MANAGER_H_
#define GFS_COMMON_CONFIG_MANAGER_H_

#include <string>
#include <vector>
#include <unordered_set>

#include "yaml-cpp/yaml.h"

namespace gfs
{
    namespace common
    {
        class ConfigManager
        {
        public:
            static ConfigManager *GetConfig(const std::string &config_filename);
            std::vector<std::string> GetAllMasterServers();
            std::vector<std::string> GetAllChunkServers();

            bool HasMasterServer(const std::string &server_name);
            bool HasChunkServer(const std::string &server_name);
            std::string GetServerHostname(const std::string &server_name);
            uint32_t GetServerPort(const std::string &server_name);
            std::string GetServerAddress(const std::string &server_name,
                                         const bool resolve_hostname = false);
            std::string ResolveHostname(const std::string &hostname);
            std::string GetDatabaseName(const std::string &server_name);
            uint64_t GetFileChunkBlockSize();
            uint64_t GetRequiredDiskSpaceToMaintain();

        private:
            ConfigManager() = default;
            ConfigManager(const YAML::Node &node);
            YAML::Node config_;
            std::unordered_set<std::string> master_servers_names_;
            std::unordered_set<std::string> chunk_servers_names_;
        };
    };
};

#endif