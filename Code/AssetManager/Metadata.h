#pragma once

#include "Platform/Guid.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace ge2::assets
{
    struct Metadata
    {
        GUID guid;
        std::wstring path;
        std::wstring name;
        std::wstring extension;
        std::vector<GUID> dependencies;

        bool loadStarted = false;   //Used to identify recursive dependencies
        bool loaded = false;
    };

    using MetadataMap = std::unordered_map<ge2::GUID, Metadata>;


    void WriteMetadata();

    GUID GUIDFromFilename(std::wstring_view const&);
    std::wstring FilenameFromGUID(GUID);

    MetadataMap& GetAllMetadata();
    Metadata* GetMetadata(GUID);
}