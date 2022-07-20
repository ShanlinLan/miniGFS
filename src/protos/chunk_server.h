#ifndef GFS_PROTOS_CHUNK_SERVER_H_
#define GFS_PROTOS_CHUNK_SERVER_H_

#include <string>
#include <vector>

struct ChunkServerLocation{
    std::string server_hostname;
    uint32_t server_port;
};

struct ChunkServer{
    ChunkServerLocation location;
    uint32_t available_disk_mb;
    std::vector<std::string> stored_chunk_handles;
};

struct FileChunk{
    uint32_t version;
    std::string data;
};

#endif