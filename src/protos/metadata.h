#ifndef GFS_PROTOS_METADATA_H_
#define GFS_PROTOS_METADATA_H_

#include <string>
#include <map>
#include <cstdint>

#include "src/protos/chunk_server.h"


struct FileMetadata{
    std::string filename;
    std::map<uint32_t, std::string> chunk_handles;  // 文件块与其存放的chunk server
};


struct FileChunkMetadata{
    std::string chunk_handle;       // chunkserver的唯一标识符
    uint32_t version;
    ChunkServerLocation primary_location;
    std::vector<ChunkServerLocation> locations;
};

#endif