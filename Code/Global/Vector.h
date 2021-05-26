#pragma once

#include <cmath>

namespace ge2
{
	template <typename TYPE>
    struct Vector2
    {
		static Vector2 Zero();
		static Vector2 UnitX();
		static Vector2 UnitY();

        float x = 0;
        float y = 0;
		
		Vector2 operator-() const;
		
		Vector2 operator+(const Vector2&) const;
		Vector2 operator-(const Vector2&) const;
		Vector2& operator+=(const Vector2&);
		Vector2& operator-=(const Vector2&);
		
		Vector2 operator*(TYPE value) const;
		Vector2 operator/(TYPE value) const;
		Vector2& operator*=(TYPE value);
		Vector2& operator/=(TYPE value);

        float ModSquared() const;
        float Magnitude() const;
        Vector2 Normalise() const;
    };

	template <typename TYPE>
    struct Vector3
    {
		static Vector3 Zero();
		static Vector3 UnitX();
		static Vector3 UnitY();
		static Vector3 UnitZ();

        float x = 0;
        float y = 0;
		float z = 0;
		
		Vector3 operator-() const;
		
		Vector3 operator+(const Vector3& rhs) const;
		Vector3 operator-(const Vector3& rhs) const;
		Vector3& operator+=(const Vector3& rhs);
		Vector3& operator-=(const Vector3& rhs);
		
		Vector3 operator*(TYPE value) const;
		Vector3 operator/(TYPE value) const;
		Vector3& operator*=(TYPE value);
		Vector3& operator/=(TYPE value);

        float ModSquared() const;
        float Magnitude() const;
        Vector3 Normalise() const;
    };

	using Vector2f = Vector2<float>;
	using Vector3f = Vector3<float>;

	//Functions

	template <typename TYPE>
	Vector3<TYPE> CrossProduct(Vector3<TYPE> const& lhs, Vector3<TYPE> const& rhs)
	{
		return Vector3<TYPE>
		{
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		};
	}
	template <typename TYPE>
	TYPE DotProduct(Vector3<TYPE> const& lhs, Vector3<TYPE> const& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z
	}

	//Vector2 definitions

	template<typename TYPE>
	Vector2<TYPE> Vector2<TYPE>::Zero()
	{
		return { 0,0 };
	}
	template<typename TYPE>
	Vector2<TYPE> Vector2<TYPE>::UnitX()
	{
		return { 1,0 };
	}
	template<typename TYPE>
	Vector2<TYPE> Vector2<TYPE>::UnitY()
	{
		return { 0,1 };
	}

	template<typename TYPE>
	Vector2<TYPE> Vector2<TYPE>::operator-() const
	{
		return { -x, -y };
	}
	template<typename TYPE>
	Vector2<TYPE> Vector2<TYPE>::operator+(const Vector2& rhs) const
	{
		return { x + rhs.x, y + rhs.y };
	}
	template<typename TYPE>
	Vector2<TYPE> Vector2<TYPE>::operator-(const Vector2& rhs) const
	{
		return { x - rhs.x, y - rhs.y };
	}
	template<typename TYPE>
	Vector2<TYPE>& Vector2<TYPE>::operator+=(const Vector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	template<typename TYPE>
	Vector2<TYPE>& Vector2<TYPE>::operator-=(const Vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	template<typename TYPE>
	Vector2<TYPE> Vector2<TYPE>::operator*(TYPE value) const
	{
		return { x * value, y * value };
	}
	template<typename TYPE>
	Vector2<TYPE> Vector2<TYPE>::operator/(TYPE value) const
	{
		return { x / value, y / value };
	}
	template<typename TYPE>
	Vector2<TYPE>& Vector2<TYPE>::operator*=(TYPE value)
	{
		x *= value;
		y *= value;
		return *this;
	}
	template<typename TYPE>
	Vector2<TYPE>& Vector2<TYPE>::operator/=(TYPE value)
	{
		x /= value;
		y /= value;
		return *this;
	}
	
	template<typename TYPE>
	float Vector2<TYPE>::ModSquared() const
	{
		return x * x + y * y;
	}
	template<typename TYPE>
	float Vector2<TYPE>::Magnitude() const
	{
		return sqrtf(ModSquared());
	}
	template<typename TYPE>
	Vector2<TYPE> Vector2<TYPE>::Normalise() const
	{
		if (*this == Zero())
			return *this;

		return *this / Magnitude();
	}
	
	//Vector3 definitions

	template<typename TYPE>
	Vector3<TYPE> Vector3<TYPE>::Zero()
	{
		return { 0,0,0 };
	}
	template<typename TYPE>
	Vector3<TYPE> Vector3<TYPE>::UnitX()
	{
		return { 1,0,0 };
	}
	template<typename TYPE>
	Vector3<TYPE> Vector3<TYPE>::UnitY()
	{
		return { 0,1,0 };
	}
	template<typename TYPE>
	Vector3<TYPE> Vector3<TYPE>::UnitZ()
	{
		return { 0,0,1 };
	}

	template<typename TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator-() const
	{
		return { -x, -y, -z };
	}
	template<typename TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator+(const Vector3& rhs) const
	{
		return { x + rhs.x, y + rhs.y, z + rhs.z };
	}
	template<typename TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator-(const Vector3& rhs) const
	{
		return { x - rhs.x, y - rhs.y, z - rhs.z };
	}
	template<typename TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator+=(const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	template<typename TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator-=(const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	template<typename TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator*(TYPE value) const
	{
		return { x * value, y * value, z * value };
	}
	template<typename TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator/(TYPE value) const
	{
		return { x / value, y / value, z / value };
	}
	template<typename TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator*=(TYPE value)
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}
	template<typename TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator/=(TYPE value)
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}
	
	template<typename TYPE>
	float Vector3<TYPE>::ModSquared() const
	{
		return x * x + y * y + z * z;
	}
	template<typename TYPE>
	float Vector3<TYPE>::Magnitude() const
	{
		return sqrtf(ModSquared());
	}
	template<typename TYPE>
	Vector3<TYPE> Vector3<TYPE>::Normalise() const
	{
		if (*this == Zero())
			return *this;

		return *this / Magnitude();
	}

}