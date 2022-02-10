#pragma once
#include <cstdint>
#include <ostream>
#include <array>
#include "Math.h"

template <typename T>
class Vector3;

template <typename T>
class Vector2
{
public:
	union
	{
		struct
		{
			T x, y;
		};
		struct
		{
			std::array<T, 2> values;
		};
	};

	Vector2();
	Vector2(const T &x, const T &y);
	Vector2(const T &value);

	inline Vector2<T> &operator+=(const T &value);
	inline Vector2<T> &operator+=(const Vector2<T> &right);
	inline Vector2<T> &operator-=(const T &value);
	inline Vector2<T> &operator-=(const Vector2<T> &right);
	inline Vector2<T> &operator*=(const T &value);
	inline Vector2<T> &operator*=(const Vector2<T> &right);
	inline Vector2<T> &operator/=(const T &value);
	inline Vector2<T> &operator/=(const Vector2<T> &right);
	inline Vector2<T> &operator=(const Vector2<T> &right);

	inline T SquareLength() const;
	inline T Length() const;

	static T Dot(const Vector2<T> &left, const Vector2<T> &right);
	static Vector2<T> Normalize(const Vector2<T> &vec);
	static T IncludedAngle(const Vector2<T> &left, const Vector2<T> &right);
	static Vector3<T> ToVector3(const Vector2<T> &right, T zValue);

	static Vector2<T> Lerp(const Vector2<T> &left, const Vector2<T> &right, T t);
	static Vector2<T> NLerp(const Vector2<T> &left, const Vector2<T> &right, T t);

	static Vector2<T> Cross(const Vector2<T> &left, T right);
	static Vector2<T> Cross(T left, const Vector2<T> &right);
	static T Cross(const Vector2<T> &left, const Vector2<T> &right);

	static const Vector2<T> ZERO;
	static const Vector2<T> UNIT_X;
	static const Vector2<T> UNIT_Y;
};

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int32_t> Vector2i32;
typedef Vector2<uint32_t> Vector2u32;
typedef Vector2<int16_t> Vector2i16;
typedef Vector2<uint16_t> Vector2u16;
typedef Vector2<int8_t> Vector2i8;
typedef Vector2<uint8_t> Vector2u8;

template <typename T>
const Vector2<T> Vector2<T>::ZERO = Vector2<T>();

template <typename T>
const Vector2<T> Vector2<T>::UNIT_X = Vector2<T>(static_cast<T>(1.0f), static_cast<T>(0.0f));

template <typename T>
const Vector2<T> Vector2<T>::UNIT_Y = Vector2<T>(static_cast<T>(0.0f), static_cast<T>(1.0f));

template <typename T>
inline Vector2<T>::Vector2()
	: values({static_cast<T>(0.0f), static_cast<T>(0.0f)})
{
}
template <typename T>
inline Vector2<T>::Vector2(const T &x, const T &y)
	: values({x, y})
{
}
template <typename T>
inline Vector2<T>::Vector2(const T &value)
	: values({value, value})
{
}

template <typename T>
inline Vector2<T> operator+(const Vector2<T> &left, const Vector2<T> &right)
{
	return Vector2<T>(left.x + right.x, left.y + right.y);
}
template <typename T>
inline Vector2<T> operator+(const Vector2<T> &left, const T &value)
{
	return Vector2<T>(left.x + value, left.y + value);
}
template <typename T>
inline Vector2<T> operator+(const T &value, const Vector2<T> &right)
{
	return right + value;
}
template <typename T>
inline Vector2<T> operator-(const Vector2<T> &left, const Vector2<T> &right)
{
	return Vector2<T>(left.x - right.x, left.y - right.y);
}
template <typename T>
inline Vector2<T> operator-(const Vector2<T> &left, const T &value)
{
	return Vector2<T>(left.x - value, left.y - value);
}
template <typename T>
inline Vector2<T> operator-(const T &value, const Vector2<T> &right)
{
	return Vector2<T>(value - right.x, value - right.y);
}
template <typename T>
inline Vector2<T> operator-(const Vector2<T> &right)
{
	return Vector2<T>(-right.x, -right.y);
}
template <typename T>
inline Vector2<T> operator*(const Vector2<T> &left, const T &value)
{
	return Vector2<T>(left.x * value, left.y * value);
}
template <typename T>
inline Vector2<T> operator*(const T &value, const Vector2<T> &right)
{
	return Vector2<T>(value * right.x, value * right.y);
}
template <typename T>
inline Vector2<T> operator*(const Vector2<T> &left, const Vector2<T> &right)
{
	return Vector2<T>(left.x * right.x, left.y * right.y);
}
template <typename T>
inline Vector2<T> operator/(const Vector2<T> &left, const T &value)
{
	if (value)
		return Vector2<T>(left.x / value, left.y / value);
	return left;
}
template <typename T>
inline Vector2<T> operator/(const T &value, const Vector2<T> &right)
{
	return right / value;
}
template <typename T>
inline Vector2<T> operator/(const Vector2<T> &left, const Vector2<T> &right)
{
	if (right.x && right.y)
		return Vector2<T>(left.x / right.x, left.y / right.y);
	return left;
}
template <typename T>
inline bool operator==(const Vector2<T> &left, const Vector2<T> &right)
{
	return (left.x - right.x) == static_cast<T>(0.0f) && (left.y - right.y) == static_cast<T>(0.0f);
}
template <typename T>
inline std::ostream &operator<<(std::ostream &stream, const Vector2<T> &vec)
{

	stream << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
	return stream;
}

template <typename T>
inline Vector2<T> &Vector2<T>::operator+=(const T &value)
{

	x += value;
	y += value;
	return *this;
}
template <typename T>
inline Vector2<T> &Vector2<T>::operator+=(const Vector2<T> &right)
{

	x += right.x;
	y += right.y;
}
template <typename T>
inline Vector2<T> &Vector2<T>::operator-=(const T &value)
{

	x -= value;
	y -= value;
	return *this;
}
template <typename T>
inline Vector2<T> &Vector2<T>::operator-=(const Vector2<T> &right)
{

	x -= right.x;
	y -= right.y;
	return *this;
}
template <typename T>
inline Vector2<T> &Vector2<T>::operator*=(const T &value)
{

	x *= value;
	y *= value;
}
template <typename T>
inline Vector2<T> &Vector2<T>::operator*=(const Vector2<T> &right)
{

	x *= right.x;
	y *= right.y;
}
template <typename T>
inline Vector2<T> &Vector2<T>::operator/=(const T &value)
{

	if (value)
	{
		x /= value;
		y /= value;
	}
	return *this;
}
template <typename T>
inline Vector2<T> &Vector2<T>::operator/=(const Vector2<T> &right)
{

	if (!Math::IsNearZero(right.x) && !Math::IsNearZero(right.y))
	{
		x /= right.x;
		y /= right.y;
	}
	return *this;
}
template <typename T>
inline Vector2<T> &Vector2<T>::operator=(const Vector2<T> &right)
{
	x = right.x;
	y = right.y;
	return *this;
}
template <typename T>
inline T Vector2<T>::SquareLength() const
{
	return x * x + y * y;
}
template <typename T>
inline T Vector2<T>::Length() const
{
	return Math::Sqrt(SquareLength());
}
template <typename T>
inline T Vector2<T>::Dot(const Vector2<T> &left, const Vector2<T> &right)
{
	return left.x * right.x + left.y * right.y;
}
template <typename T>
inline Vector2<T> Vector2<T>::Normalize(const Vector2<T> &vec)
{
	T len = vec.Length();
	if (!Math::IsNearZero(len))
		return Vector2<T>(vec.x / len, vec.y / len);
	return Vector2<T>::ZERO;
}
template <typename T>
inline T Vector2<T>::IncludedAngle(const Vector2<T> &left, const Vector2<T> &right)
{
	return Math::ArcCos(Vector2<T>::Dot(Vector2<T>::Normalize(left), Vector2<T>::Normalize(right)));
}
template <typename T>
inline Vector3<T> Vector2<T>::ToVector3(const Vector2<T> &right, T zValue)
{
	return Vector3<T>(right, zValue);
}

template <typename T>
inline Vector2<T> Vector2<T>::Lerp(const Vector2<T> &left, const Vector2<T> &right, T t)
{
	return (1.0f - t) * left + t * right;
}

template <typename T>
inline Vector2<T> Vector2<T>::NLerp(const Vector2<T> &left, const Vector2<T> &right, T t)
{
	return Vector3<T>::Normalize(Lerp(left, right, t));
}

template <typename T>
inline Vector2<T> Vector2<T>::Cross(const Vector2<T> &left, T right)
{
	return Vector2<T>(right * left.y, -right * left.x);
}
template <typename T>
inline Vector2<T> Vector2<T>::Cross(T left, const Vector2<T> &right)
{
	return Vector2<T>(-left * right.y, left * right.x);
}
template <typename T>
inline T Vector2<T>::Cross(const Vector2<T> &left, const Vector2<T> &right)
{
	return left.x * right.y - right.x * left.y;
}