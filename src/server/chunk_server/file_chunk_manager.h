#ifndef GFS_SERVER_CHUNK_SERVER_FILE_CHUNK_MANAGER_H_
#define GFS_SERVER_CHUNK_SERVER_FILE_CHUNK_MANAGER_H_

#include <memory>
#include <string>

#include "leveldb/db.h"
#include "src/gfs/common/utils.h"

namespace gfs
{

    namespace chunkserver
    {

        class FileChunkManager
        {

        public:
            static FileChunkManager *GetInstance();

            FileChunkManager(const FileChunkManager &) = delete;

            void operator=(const FileChunkManager &) = delete;

            void Initialize(const std::string &chunk_database_name, const uint32_t &max_chunk_size_bytes);

            uint32_t GetMaxChunkSizeBytes() const;

        private:
            // 使用leveldb存储chunk
            std::unique_ptr<leveldb::DB> chunk_database_;

            // chunk块的大小
            uint32_t max_chunk_size_bytes_;

            FileChunkManager();
        };

    }
}

#endif