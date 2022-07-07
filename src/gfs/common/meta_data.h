// 2022/5/25
// 各种元数据的结构

#include <string>
#include <map>
#include <cstdint>

// 文件的元数据结构
struct FileMetadata{
    std::string filename;
    std::map<uint32_t, std::string> chunk_handles;  // 文件块与其存放的chunk server
};

struct ChunkServerLocation{
    std::string server_hostname;
    uint32_t server_port;
};

struct ChunkServer{
    ChunkServerLocation location;
    uint32_t available_disk_mb;
    std::string stored_chunk_handles;
};

struct FileChunk{
    uint32_t version;

};

struct FileChunkMetadata{
    std::string chunk_handle;       // chunkserver的唯一标识符
    uint32_t version;
    ChunkServerLocation primary_location;
    ChunkServerLocation locations;
};