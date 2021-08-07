#pragma once

#include "Graphics/Core.h"
#include "Platform/Guid.h"

namespace ge2
{

    struct Scene
    {
        GUID id;
        gfx::Core graphics;

        void update();
    };

}