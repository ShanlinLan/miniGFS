#include "src/server/chunk_server/file_chunk_manager.h"

#include <memory>
#include <string>

#include "leveldb/db.h"
#include "src/common/utils.h"
#include "src/common/system_logger.h"

namespace gfs
{

    namespace server
    {

        FileChunkManager::FileChunkManager()
            : chunk_database_(nullptr), max_chunk_size_bytes_(0) {}

        FileChunkManager *FileChunkManager::GetInstance()
        {
            static FileChunkManager instance;
            return &instance;
        }

        void FileChunkManager::Initialize(const std::string &chunk_database_name,
                                          const uint32_t &max_chunk_size_bytes)
        {
            leveldb::DB *database;
            leveldb::Options options;

            options.create_if_missing = true;
            leveldb::Status status = leveldb::DB::Open(options, chunk_database_name, &database);

            LOG_ASSERT(status.ok()) << status.ToString();

            this->chunk_database_ = std::unique_ptr<leveldb::DB>(database);
            this->max_chunk_size_bytes_ = max_chunk_size_bytes;
        }

        uint32_t FileChunkManager::GetMaxChunkSizeBytes() const{
            return this->max_chunk_size_bytes_;
        }

        

    }
}