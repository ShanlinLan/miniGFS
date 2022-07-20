#include "utils.h"

namespace gfs
{
    namespace common
    {
        namespace utils
        {
            StatusCode ValidateConfigFile(const YAML::Node &node)
            {
                if (!node.IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["servers"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["network"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["disk"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["timeout"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["servers"]["master_servers"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["servers"]["chunk_servers"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["network"]["dns_lookup_table"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["disk"]["block_size_mb"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["disk"]["min_free_disk_space_mb"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["disk"]["leveldb"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["timeout"]["grpc"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["timeout"]["lease"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["timeout"]["heartbeat"].IsDefined())
                {
                    return InvalidArgumentError;
                }
                else if (!node["timeout"]["client_cache"].IsDefined())
                {
                    return InvalidArgumentError;
                }

                std::vector<std::string> server_types = {"master_servers", "chunk_servers"};
                for (std::string &server_type : server_types)
                {
                    for (size_t i = 0; i < node["servers"][server_type].size(); i++)
                    {
                        std::string server_name = node["servers"][server_type][i].as<std::string>();
                        if (!node["network"][server_name].IsDefined())
                        {
                            return InvalidArgumentError;
                        }
                        else if (!node["network"][server_name]["hostname"].IsDefined())
                        {
                            return InvalidArgumentError;
                        }
                        else if (!node["network"][server_name]["port"].IsDefined())
                        {
                            return InvalidArgumentError;
                        }
                        else if (server_type == "chunk_servers" && !node["disk"]["leveldb"][server_name].IsDefined())
                        {
                            return InvalidArgumentError;
                        }
                        std::string hostname = node["network"][server_name]["hostname"].as<std::string>();
                        if (!node["network"]["dns_lookup_table"][hostname].IsDefined())
                        {
                            return InvalidArgumentError;
                        }
                    }
                }
                return OK;
            }
        };
    };
};