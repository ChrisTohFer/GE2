#pragma once

#include "Global/Transform.h"

namespace ge2
{
    struct Camera
    {
        Transform* transform;

        void LookAt(Vector3f location);
        Matrix4x4f Matrix() const;
    };
}