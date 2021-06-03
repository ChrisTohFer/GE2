#pragma once

#include "Constants.h"
#include "Vector.h"
#include "Matrix.h"

namespace ge2
{
    struct Quaternion
    {
        float       w;
        Vector3f    v;

        static constexpr Quaternion Identity();
        static Quaternion FromEuler(Vector3f const&);

        void Euler(Vector3f const&);
        Vector3f Euler() const;

        Matrix4x4 RotationMatrix() const;

        Quaternion Inverse() const;
        Quaternion Normalised() const;

        Quaternion operator*(Quaternion const&) const;
        Quaternion& operator*=(Quaternion const&);
    };

    //Function definitions

    inline Vector3f operator*(Quaternion const& q, Vector3f const& v)
    {
        auto result = q * Quaternion{ 0, v } *q.Inverse();
        return result.v;
    }

    //Quaternion function definitions

    inline constexpr Quaternion Quaternion::Identity()
    {
        return Quaternion{
            1.f,
            Vector3f::Zero()
        };
    }

    //Euler angle are applied in the order yaw(y), pitch(x), roll(z)
    inline Quaternion Quaternion::FromEuler(Vector3f const& euler)
    {
        // Abbreviations for the various angular functions
        float cx = cosf(euler.x * 0.5f);
        float sx = sinf(euler.x * 0.5f);
        float cy = cosf(euler.y * 0.5f);
        float sy = sinf(euler.y * 0.5f);
        float cz = cosf(euler.z * 0.5f);
        float sz = sinf(euler.z * 0.5f);

        Quaternion q;
        q.w = cx * cy * cz - sx * sy * sz;
        q.v.x = sx * cy * cz - cx * sy * sz;
        q.v.y = cx * sy * cz + sx * cy * sz;
        q.v.z = sx * sy * cz + cx * cy * sz;
        return q;
    }

    //Euler angle are applied in the order yaw(y), pitch(x), roll(z)
    inline void Quaternion::Euler(Vector3f const& euler)
    {
        *this = FromEuler(euler);
    }

    //Euler angle are applied in the order yaw(y), pitch(x), roll(z)
    inline Vector3f Quaternion::Euler() const
    {
        Vector3f euler;

        // pitch (x-axis rotation)
        float sinx = 2.f * (v.y * v.z + w * v.x);
        if (fabsf(sinx) >= 1.f)
            euler.x = std::copysign(PI / 2.f, sinx); // use 90 degrees if out of range
        else
            euler.x = asinf(sinx);

        // pitch (y-axis rotation)
        euler.y = atan2f(-2.f * (v.x * v.z - w * v.y), 2.f * (w * w + v.z * v.z) - 1.f);

        // yaw (z-axis rotation)
        euler.z = atan2f(-2.f * (v.x * v.y - w * v.z), 2.f * (w*w + v.y * v.y) - 1.f);

        return euler;
    }

    inline Matrix4x4 Quaternion::RotationMatrix() const
    {
        return Matrix4x4
        {
            2 * (w * w + v.x * v.x) - 1, 2 * (v.x * v.y - w * v.z), 2 * (v.x * v.z + w * v.y), 0.0f,
            2 * (v.x * v.y + w * v.z), 2 * (w * w + v.y * v.y) - 1, 2 * (v.y * v.z - w * v.x), 0.0f,
            2 * (v.x * v.z - w * v.y), 2 * (v.y * v.z + w * v.x), 2 * (w * w + v.z * v.z) - 1, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    inline Quaternion Quaternion::Inverse() const
    {
        float divisor = w * w + v.ModSquared();
        return Quaternion
        {
            w / divisor,
            -v / divisor
        };
    }

    inline Quaternion Quaternion::Normalised() const
    {
        float divisor = sqrtf(w * w + v.ModSquared());

        if (divisor == 0)
        {
            return Quaternion::Identity();  //Maybe this should be a zero quaternion?
        }

        return Quaternion
        {
            w / divisor,
            v / divisor
        };
    }

    inline Quaternion Quaternion::operator*(Quaternion const& rhs) const
    {
        return Quaternion
        {
            w * rhs.w - v.x * rhs.v.x - v.y * rhs.v.y - v.z * rhs.v.z,
            Vector3f
            {
                w * rhs.v.x + v.x * rhs.w + v.y * rhs.v.z - v.z * rhs.v.y,
                w * rhs.v.y - v.x * rhs.v.z + v.y * rhs.w + v.z * rhs.v.x,
                w * rhs.v.z + v.x * rhs.v.y - v.y * rhs.v.x + v.z * rhs.w
            }
        };
    }

    inline Quaternion& Quaternion::operator*=(Quaternion const& rhs)
    {
        *this = *this * rhs;
        return *this;
    }

}