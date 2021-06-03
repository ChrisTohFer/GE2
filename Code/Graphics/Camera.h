#pragma once

#include "Global/Vector.h"
#include "Global/Quaternion.h"

namespace ge2
{

    struct Camera
    {
        Vector3f position;
        Quaternion rotation;

        void LookAt(Vector3f location);
    };

}