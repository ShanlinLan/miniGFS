#ifndef GFS_SERVER_CHUNK_SERVER_FILE_CHUNK_MANAGER_H_
#define GFS_SERVER_CHUNK_SERVER_FILE_CHUNK_MANAGER_H_

#include <map>
#include <memory>
#include <string>

#include "leveldb/db.h"
#include "src/common/utils.h"
#include "src/protos/chunk_server.h"
#include "src/protos/metadata.h"

namespace gfs {

namespace server {

class FileChunkManager {
 public:
  static FileChunkManager *GetInstance();

  FileChunkManager(const FileChunkManager &) = delete;

  void operator=(const FileChunkManager &) = delete;

  void Initialize(const std::string &chunk_database_name,
                  const uint32_t &max_chunk_size_bytes);

  uint32_t GetMaxChunkSizeBytes() const;

  std::list<FileChunkMetadata> GetAllFileChunkMetadata();

  StatusCode CreateChunk(const std::string &chunk_handle,
                         const uint32_t &create_version);

  std::pair<StatusCode, std::string> ReadFromChunk(
      const std::string &chunk_handle, const uint32_t &read_version,
      const uint32_t &start_offset, const uint32_t &length);

  std::pair<StatusCode, uint32_t> WriteToChunk(const std::string &chunk_handle,
                                               const uint32_t &write_version,
                                               const uint32_t &start_offset,
                                               const uint32_t &length,
                                               const std::string &new_data);

  StatusCode UpdateChunkVersion(const std::string &chunk_handle,
                                const uint32_t &from_version,
                                const uint32_t &to_version);

  std::pair<StatusCode, uint32_t> GetChunkVersion(
      const std::string &chunk_handle);

  std::pair<StatusCode, uint32_t> AppendToChunk(const std::string &chunk_handle,
                                                const uint32_t &append_version,
                                                const uint32_t &length,
                                                const std::string &new_data);

  std::pair<StatusCode, uint32_t> DeleteChunk(const std::string &chunk_handle);

 private:
  std::pair<StatusCode, std::shared_ptr<FileChunk>> GetFileChunk(
      const std::string &chunk_handle);

  std::pair<StatusCode, std::shared_ptr<FileChunk>> GetFileChunk(
      const std::string &chunk_handle, const uint32_t &version);

  leveldb::Status WriteFileChunk(const std::string &chunk_handle,
                                 const FileChunk *file_chunk);

  // 使用leveldb存储chunk
  std::unique_ptr<leveldb::DB> chunk_database_;

  // chunk块的大小
  uint32_t max_chunk_size_bytes_;

  FileChunkManager();
};

}  // namespace server
}  // namespace gfs

#endif