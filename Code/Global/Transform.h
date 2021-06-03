#pragma once

#include "Matrix.h"
#include "Quaternion.h"
#include "Vector.h"

namespace ge2
{
    struct Transform
    {
        Vector3f position;
        Vector3f scale;
        Quaternion rotation;

        Matrix4x4f Matrix() const;
    };

    Matrix4x4f Transform::Matrix() const
    {
        //Rotation
        Matrix4x4f result = rotation.RotationMatrix();

        //Scale
        result = result * Matrix4x4f{
            scale.x,    0.f,        0.f,        0.f,
            0.f,        scale.y,    0.f,        0.f,
            0.f,        0.f,        scale.z,    0.f,
            0.f,        0.f,        0.f,        1.f,
        };

        //Position
        result.Get(0, 3) = position.x;
        result.Get(1, 3) = position.y;
        result.Get(2, 3) = position.z;

        return result;
    }
}