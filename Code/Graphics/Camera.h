#pragma once

#include "Global/Constants.h"
#include "Global/Transform.h"

namespace ge2
{
    struct Camera
    {
        Transform transform;
        float fovY = PI / 4.f;             //Field of view
        float near = 0.1f;
        float far  = 100.f;

        void LookAt(Vector3f location);
        Matrix4x4f Matrix() const;
    };
}