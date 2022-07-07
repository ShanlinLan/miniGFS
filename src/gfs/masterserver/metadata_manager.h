#pragma once

#include <string>
#include <atomic>
#include <unordered_map>

#include "../common/meta_data.h"
#include "../common/utils.h"

namespace masterserver{
class MetadataManager
{
public:
    static MetadataManager* GetInstance();

    StatusCode CreateFileMetadata(const std::string &filename);

    bool ExistFileMetadata(const std::string &filename);

    // void DeleteFileAndChunkMetadata(const std::string &filename);

    // FileMetadata *GetFileMetadata(const std::string &filename);

    // std::string CreateChunkHandle(const std::string &filename, uint32_t chunk_index);

    // std::string GetChunkHandle(const std::string &filename, uint32_t chunk_index);

private:
    MetadataManager();

    std::atomic<uint64_t> global_chunk_id_{0};

    std::unordered_map<std::string, FileMetadata*> file_metadata_;

    std::unordered_map<std::string, FileChunkMetadata*> chunk_metadata_;

    std::unordered_map<std::string, std::pair<ChunkServerLocation*, uint64_t>> lease_holders_;
};
};