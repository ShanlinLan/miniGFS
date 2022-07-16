#include "src/gfs/chunkserver/file_chunk_manager.h"

#include <memory>
#include <string>

#include "leveldb/db.h"
#include "src/gfs/common/utils.h"

namespace gfs
{

    namespace chunkserver
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
            leveldb::DB* database;
            leveldb::Options options;

            options.create_if_missing = true;
            leveldb::Status status = leveldb::DB::Open(options, chunk_database_name, &database);
        }

    }
}