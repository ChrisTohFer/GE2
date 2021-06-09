#pragma once

#include "Platform/Guid.h"

#include <string_view>

namespace ge2::assets
{
    void LoadMetadata();
    void UpdateMetadata();

    GUID GUIDFromFilename(std::wstring_view const&);
    std::wstring FilenameFromGUID(GUID);
}