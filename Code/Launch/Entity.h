#pragma once

#include "Global/Flags.h"
#include "Global/Transform.h"
#include "Platform/Guid.h"

namespace ge2
{
    enum class ComponentType
    {
        RENDERER,
        PHYSICS,

        NUM_TYPES
    };
    using ComponentFlags = Flags<ComponentType>;

    struct Entity
    {
        GUID            guid;
        ComponentFlags  componentFlags;
        Transform       transform;
    };


}