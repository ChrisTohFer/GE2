#pragma once

#include "Loader.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace ge2::ast
{
    void AddLoader(Loader& loader);
    void RemoveLoader(Loader const& loader);
    void LoadAllAssets();
}