#pragma once

#include "Platform/Guid.h"

namespace ge2
{

    struct Scene
    {
        GUID id;
        //System members

        void update();
    };

}