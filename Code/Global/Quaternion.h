#pragma once

#include "Constants.h"
#include "Vector.h"

namespace ge2
{
    struct Quaternion
    {
        float       scalar;
        Vector3f    vector;

        void Euler(Vector3f const&);
        Vector3f Euler() const;

        Quaternion Inverse() const;
        Quaternion Normalised() const;

        Quaternion operator*(Quaternion const&) const;
        Quaternion& operator*=(Quaternion const&);
    };

    //Function definitions

    inline void Quaternion::Euler(Vector3f const& euler)
    {
        //From wikipedia

        // Abbreviations for the various angular functions
        float cy = cosf(euler.y * 0.5);
        float sy = sinf(euler.y * 0.5);
        float cp = cosf(euler.z * 0.5);
        float sp = sinf(euler.z * 0.5);
        float cr = cosf(euler.x * 0.5);
        float sr = sinf(euler.x * 0.5);

        Quaternion& q = *this;
        q.scalar = cr * cp * cy + sr * sp * sy;
        q.vector.x = sr * cp * cy - cr * sp * sy;
        q.vector.y = cr * sp * cy + sr * cp * sy;
        q.vector.z = cr * cp * sy - sr * sp * cy;
    }

    inline Vector3f Quaternion::Euler() const
    {
        //From wikipedia

        Vector3f euler;

        // roll (x-axis rotation)
        float sinr_cosp = 2 * (scalar * vector.x + vector.y * vector.z);
        float cosr_cosp = 1 - 2 * (vector.x * vector.x + vector.y * vector.y);
        euler.z = std::atan2f(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        float sinp = 2 * (scalar * vector.y - vector.z * vector.x);
        if (std::fabsf(sinp) >= 1)
            euler.x = std::copysignf(PI / 2, sinp); // use 90 degrees if out of range
        else
            euler.x = std::asinf(sinp);

        // yaw (z-axis rotation)
        float siny_cosp = 2 * (scalar * vector.z + vector.x * vector.y);
        float cosy_cosp = 1 - 2 * (vector.y * vector.y + vector.z * vector.z);
        euler.y = std::atan2f(siny_cosp, cosy_cosp);

        return euler;
    }

    inline Quaternion Quaternion::Inverse() const
    {
        float divisor = scalar * scalar + vector.ModSquared();
        return Quaternion
        {
            scalar / divisor,
            vector / divisor
        };
    }

    inline Quaternion Quaternion::Normalised() const
    {
        float divisor = scalar + vector.Magnitude();
        return Quaternion
        {
            scalar / divisor,
            vector / divisor
        };
    }

    inline Quaternion Quaternion::operator*(Quaternion const& rhs) const
    {
        return Quaternion
        {
            scalar * rhs.scalar - vector.x * rhs.vector.x - vector.y * rhs.vector.y - vector.z * rhs.vector.z,
            Vector3f
            {
                scalar * rhs.vector.x + vector.x * rhs.scalar + vector.y * rhs.vector.z - vector.z * rhs.vector.y,
                scalar * rhs.vector.y - vector.x * rhs.vector.z + vector.y * rhs.scalar + vector.z * rhs.vector.x,
                scalar * rhs.vector.z + vector.x * rhs.vector.y - vector.y * rhs.vector.x + vector.z * rhs.scalar
            }
        };
    }

    inline Quaternion& Quaternion::operator*=(Quaternion const& rhs)
    {
        *this = *this * rhs;
        return *this;
    }

}