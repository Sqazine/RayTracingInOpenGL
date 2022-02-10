#pragma once
#include <cmath>
namespace Math
{
	constexpr float PI = 3.1415926535f;

	template<typename T>
	inline T ToRadian(const T& degree)
	{
		return degree * PI / 180.0f;
	}

	template<typename T>
	inline T ToDegree(const T& radian)
	{
		return radian * 180 / PI;
	}

	template<typename T>
	inline bool IsNearZero(const T& value,const T& epsilon=0.0000001f)
	{
		if (abs(value) <= epsilon)
			return true;
		else return false;
	}

	template<typename T>
	inline T Floor(const T& value)
	{
		return std::floor(value);
	}

	template<typename T>
	inline T Ceil(const T& value)
	{
		return ceil(value);
	}
	

	template<typename T>
	inline T Round(const T& value)
	{
		return round(value);
	}

	template<typename T>
	inline T Abs(const T& value)
	{
		return abs(value);
	}

	template<typename T>
	inline T Max(const T& left, const T& right)
	{
		return left > right ? left : right;
	}

	template<typename T>
	inline T Min(const T& left, const T& right)
	{
		return left > right ? right : left;
	}

	inline bool IsOddNumber(int value)
	{
		return value / 2 == 1 ? true : false;
	}

	inline bool IsEvenNumber(int value)
	{
		return value / 2 == 0 ? true : false;
	}

	template<typename T>
	inline T Clamp(const T& value, const T& minNum, const T& maxNum)
	{
		return  Min(maxNum, Max(minNum, value));
	}

	template<typename T>
	inline T Mix(const T& src, const T& dst, const T& percent)
	{
		return (1 - percent) * src + percent * dst;
	}

	template<typename T1,typename T2>
	inline T1 Pow(const T1& radix, const T2& power)
	{
		return pow(radix, power);
	}

	template<typename T>
	inline T Sqrt(const T& value)
	{
		return sqrt(value);
	}

	template<typename T>
	inline T Sin(const T& radian)
	{
		return sin(radian);
	}

	template<typename T>
	inline T Cos(const T& radian)
	{
		return cos(radian);
	}

	template<typename T>
	inline T Tan(const T& radian)
	{
		return tan(radian);
	}

	template<typename T>
	inline T Cot(const T& radian)
	{
		return 1.0f / Tan(radian);
	}

	template<typename T>
	inline T ArcSin(const T& value)
	{
		return asin(value);
	}

	template<typename T>
	inline T ArcCos(const T& value)
	{
		return acos(value);
	}

	template<typename T>
	inline T ArcTan(const T& value)
	{
		return atan(value);
	}

	template<typename T>
	inline T ArcTan2(const T& value)
	{
		return atan2(value);
	}

	template<typename T>
	inline T Lerp(const T& src, const T& dst, const T& percent)
	{
		return src + (dst - src) * percent;
	}

};

