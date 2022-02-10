#pragma once
#include <array>
#include <cstdint>
#include "Math.h"
template<typename T>
class Vector3;

template<typename T>
class Vector4
{
public:
	union
	{
		struct
		{
			T x, y, z, w;
		}; 
		struct
		{
			std::array<T, 4> values;
		};
	};

	Vector4();
	Vector4(const T& value);
	Vector4(const T& x, const T& y, const T& z, const T& w = static_cast<T>(1.0f));
	Vector4(const Vector3<T>& vec, const T& w = 1.0f);

	Vector4<T>& operator+=(const T& value);
	Vector4<T>& operator+=(const Vector4<T>& right);
	Vector4<T>& operator-=(const T& value);
	Vector4<T>& operator-=(const Vector4<T>& right);
	Vector4<T>& operator/=(const T& value);
	Vector4<T>& operator=(const Vector4<T>& right);

	static Vector3<T> ToVector3(const Vector4<T>& vec);

	static const Vector4<T> ZERO;
};

typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
typedef Vector4<int32_t> Vector4i32;
typedef Vector4<uint32_t> Vector4u32;
typedef Vector4<int16_t> Vector4i16;
typedef Vector4<uint16_t> Vector4u16;
typedef Vector4<int8_t> Vector4i8;
typedef Vector4<uint8_t> Vector4u8;

template<typename T>
const Vector4<T> Vector4<T>::ZERO=Vector4<T>();

template<typename T>
inline Vector4<T>::Vector4()
	:values({ static_cast<T>(0.0f),static_cast<T>(0.0f),static_cast<T>(0.0f),static_cast<T>(1.0f) })
{
}

template<typename T>
inline Vector4<T>::Vector4(const T& value)
	: values({ value,value,value,value })
{
}

template<typename T>
inline Vector4<T>::Vector4(const T& x, const T& y, const T& z, const T& w)
	: values({ x,y,z,w })
{
}

template<typename T>
inline Vector4<T>::Vector4(const Vector3<T>& vec, const T& w)
	: values({ vec.x,vec.y,vec.z,w })
{
}

template<typename T>
inline Vector4<T> operator+(const Vector4<T>& left, const Vector4<T>& right)
{
	return Vector4<T>(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}

template<typename T>
inline Vector4<T> operator+(const Vector4<T>& left, const T& value)
{
	return Vector4<T>(left.x+value,left.y+value,left.z+value,left.w+value);
}

template<typename T>
inline Vector4<T> operator+(const T& value, const Vector4<T>& right)
{
	return right+value;
}

template<typename T>
inline Vector4<T> operator-(const Vector4<T>& left, const Vector4<T>& right)
{
	return Vector4<T>(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
}

template<typename T>
inline Vector4<T> operator-(const Vector4<T>& left, const T& value)
{
	return Vector4<T>(left.x - value, left.y-value,left.z-value,left.w-value);
}

template<typename T>
inline Vector4<T> operator-(const T& value, const Vector4<T>& right)
{
	return Vector4<T>(value- right.x,value- right.y,value- right.z,value-right.w);
}

template<typename T>
inline Vector4<T> operator-(const Vector4<T>& right)
{
	return Vector4<T>(-right.x, -right.y, -right.z, -right.w);
}

template<typename T>
inline Vector4<T> operator*(const Vector4<T>& left, const Vector4<T>& right)
{
	return Vector4<T>(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
}

template<typename T>
inline Vector4<T> operator*(const T& value, const Vector4<T>& right)
{
	return Vector4<T>(value * right.x, value * right.y, value * right.z, value * right.w);
}

template<typename T>
inline Vector4<T> operator*(const Vector4<T>& left, T value)
{
	return value * left;
}

template<typename T>
inline Vector4<T> operator/(const Vector4<T>& left, T value)
{
	if (!Math::IsNearZero(value))
		return Vector4<T>(left.x / value, left.y / value, left.z / value, left.w / value);
	return left;
}

template<typename T>
inline bool operator==(const Vector4<T>& left, const Vector4<T>& right)
{
	return !(Math::IsNearZero(left.x - right.x) || Math::IsNearZero(left.y - right.y) || Math::IsNearZero(left.z - right.z) || Math::IsNearZero(left.w - right.w));
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator+=(const T& value)
{
	
	x += value;
	y += value;
	z += value;
	w += value;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& right)
{
	
	x += right.x;
	y += right.y;
	z += right.z;
	w += right.w;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator-=(const T& value)
{
	
	x -= value;
	y -= value;
	z -= value;
	w -= value;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& right)
{
	
	x -= right.x;
	y -= right.y;
	z -= right.z;
	w -= right.w;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator/=(const T& value)
{
	
	if (!Math::IsNearZero(value))
	{
		x /= value;
		y /= value;
		z /= value;
		w /= value;
	}
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator=(const Vector4<T>& right)
{
	
	x = right.x;
	y = right.y;
	z = right.z;
	w = right.w;
	return *this;
}

template<typename T>
inline Vector3<T> Vector4<T>::ToVector3(const Vector4<T>& vec)
{
	return Vector3<T>(vec.x,vec.y,vec.z);
}
