#pragma once

#include "Loader.h"
#include "Metadata.h"

namespace ge2::assets
{
    void LoadFile(Metadata& metadata);
    void AddLoader(LoaderBase& loader, std::wstring_view const& extension);
    void LoadAllAssets();
}