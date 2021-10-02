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

        //Determined at runtime
        std::wstring path;
        std::wstring extension;
        bool missing = false;

        //
        bool loaded = false;
    };

    using MetadataMap = std::unordered_map<ge2::GUID, Metadata>;

    void RefreshMetadata();
    void WriteMetadata();

    GUID GUIDFromFilename(std::wstring_view const&);
    GUID GUIDFromAsset(void*);
    std::wstring FilenameFromGUID(GUID);

    MetadataMap& GetAllMetadata();

    //Prefer storing GUIDs over pointers - pointers may be invalidated when refreshing metadata
    Metadata* GetMetadata(std::wstring_view const&);
    Metadata* GetMetadata(GUID);
    Metadata* GetMetadata(void*);   //Get metadata from pointer to asset

    Metadata* AddMetadata();
    void RemoveMetadata(GUID);
}