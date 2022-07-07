#include "../../include/masterserver/metadata_manager.h"

namespace masterserver
{
    MetadataManager::MetadataManager()
    {
    }

    MetadataManager *MetadataManager::GetInstance()
    {
        static MetadataManager *metadataManager = new MetadataManager();
        return metadataManager;
    }

    StatusCode MetadataManager::CreateFileMetadata(const std::string &filename)
    {
        if (ExistFileMetadata(filename))
        {
            return STATUS_CODE_ALREADY_EXISTS;
        }
        FileMetadata new_file_metadata;
        new_file_metadata.filename = filename;
        file_metadata_[filename] = &new_file_metadata;
        return STATUS_CODE_OK;
    }

    bool MetadataManager::ExistFileMetadata(const std::string &filename)
    {
        return file_metadata_.count(filename);
    }
};