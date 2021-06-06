#pragma once

#include "Loader.h"

namespace ge2::assets
{
    void AddLoader(LoaderBase& loader, std::wstring_view const& extension);
    void LoadAllAssets();
}