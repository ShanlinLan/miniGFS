// 2022/5/23
// master server提供的rpc服务

#include "../../include/masterserver/metadata_service.h"

using namespace masterserver;

inline MetadataManager *
MetadataServiceImplement::metadata_manager ()
{
  return MetadataManager::GetInstance ();
}

void
MetadataServiceImplement::HandleFileCreation (TinyJson &request,
                                              TinyJson &result)
{
  std::string filename = request.Get<std::string> ("filename");
  StatusCode ret
      = MetadataManager::GetInstance ()->CreateFileMetadata (filename);
  result["method"].Set ("HandleFileCreation");
  result["status"].Set (ret);
}
