#pragma once

#include "Loader.h"
#include "Metadata.h"

namespace ge2::assets
{
    using LoaderMap = std::unordered_map<std::wstring_view, LoaderBase*>;

    LoaderMap& Loaders();
    void LoadFile(Metadata& metadata);
    void AddLoader(LoaderBase& loader, std::wstring_view const& extension);
    void LoadAllAssets();
}