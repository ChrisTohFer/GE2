#pragma once

#include "Loader.h"

namespace ge2::assets
{
    void AddLoader(Loader& loader);
    void RemoveLoader(Loader const& loader);
    void LoadAllAssets();
}