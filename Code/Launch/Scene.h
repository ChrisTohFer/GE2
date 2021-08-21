#pragma once

#include "Graphics/Core.h"
#include "Platform/Guid.h"
#include "Entity.h"

#include <vector>

namespace ge2
{

    struct Scene
    {
        GUID                id;
        gfx::Core           graphics;
        std::vector<Entity> entities;

        void Update();
        void Draw(Camera const&, int screenX, int screenY);
    };

}