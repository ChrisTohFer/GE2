#pragma once

#include "Graphics/Camera.h"
#include "Platform/Guid.h"
#include "Entity.h"

#include <vector>

namespace ge2
{

    struct Scene
    {
        GUID                id;
        std::vector<Entity> entities;

        void Update();
        void Draw(Camera const&, int screenX, int screenY);
    };

}