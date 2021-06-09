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
        //Read from metadata file
        GUID guid;
        std::wstring name;
        std::vector<GUID> dependencies;

        //Determined at runtime
        std::wstring path;
        std::wstring extension;
        bool missing = false;

        //Used during loading
        bool loadStarted = false;   //Used to identify recursive dependencies
        bool loaded = false;
    };

    using MetadataMap = std::unordered_map<ge2::GUID, Metadata>;


    void WriteMetadata();

    GUID GUIDFromFilename(std::wstring_view const&);
    std::wstring FilenameFromGUID(GUID);

    MetadataMap& GetAllMetadata();

    //Prefer storing GUIDs over pointers - pointers may be invalidated when refreshing metadata
    Metadata* GetMetadata(GUID);

    Metadata* AddMetadata();
    void RemoveMetadata(GUID);
}