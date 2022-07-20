#include "config_manager.h"
#include "utils.h"

namespace gfs
{
    namespace common
    {
        ConfigManager *ConfigManager::GetConfig(const std::string &config_name)
        {
            YAML::Node node;
            try
            {
                node = YAML::LoadFile(config_name);
                StatusCode validation_status = utils::ValidateConfigFile(node);
                if (validation_status == OK)
                {
                    return new ConfigManager(node);
                }
                else
                {
                    return nullptr;
                }
            }
            catch (YAML::ParserException &e)
            {
                return nullptr;
            }
            catch (YAML::BadFile &e)
            {
                return nullptr;
            }
        }

        ConfigManager::ConfigManager(const YAML::Node &node)
        {
            config_ = node;
            for (size_t i = 0; i < config_["servers"]["master_servers"].size(); i++)
            {
                master_servers_names_.insert(
                    config_["servers"]["master_servers"][i].as<std::string>());
            }
            for (size_t i = 0; i < config_["servers"]["chunk_servers"].size(); i++)
            {
                chunk_servers_names_.insert(
                    config_["servers"]["chunk_servers"][i].as<std::string>());
            }
        }

        std::vector<std::string> ConfigManager::GetAllMasterServers()
        {
            return std::vector<std::string>(master_servers_names_.begin(),
                                            master_servers_names_.end());
        }

        std::vector<std::string> ConfigManager::GetAllChunkServers()
        {
            return std::vector<std::string>(chunk_servers_names_.begin(),
                                            chunk_servers_names_.end());
        }

        bool ConfigManager::HasMasterServer(const std::string &server_name)
        {
            return master_servers_names_.count(server_name);
        }

        bool ConfigManager::HasChunkServer(const std::string &server_name)
        {
            return chunk_servers_names_.count(server_name);
        }

        std::string ConfigManager::GetServerHostname(const std::string &server_name)
        {
            return config_["network"][server_name]["hostname"].as<std::string>();
        }

        uint32_t ConfigManager::GetServerPort(const std::string &server_name)
        {
            return config_["network"][server_name]["port"].as<uint32_t>();
        }

        std::string ConfigManager::ResolveHostname(const std::string &hostname)
        {
            return config_["network"]["dns_lookup_table"][hostname].as<std::string>();
        }

        std::string ConfigManager::GetServerAddress(const std::string &server_name,
                                                    const bool resolve_hostname)
        {
            std::string hostname = GetServerHostname(server_name);
            if (resolve_hostname)
            {
                std::string server_address = ResolveHostname(hostname);
                server_address.push_back(':');
                server_address += std::to_string(GetServerPort(server_name));
                return server_address;
            }
            else
            {
                std::string server_address = hostname;
                server_address.push_back(':');
                server_address += std::to_string(GetServerPort(server_name));
                return server_address;
            }
        }

        std::string ConfigManager::GetDatabaseName(const std::string &server_name)
        {
            return config_["disk"]["leveldb"][server_name].as<std::string>();
        }

        uint64_t ConfigManager::GetFileChunkBlockSize()
        {
            return config_["disk"]["block_size_mb"].as<uint64_t>();
        }

        uint64_t ConfigManager::GetRequiredDiskSpaceToMaintain()
        {
            return config_["disk"]["min_free_disk_space_mb"].as<uint64_t>();
        }
    };
};