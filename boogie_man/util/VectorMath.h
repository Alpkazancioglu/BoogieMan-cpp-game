#pragma once
#include <random>
#include <time.h>

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

typedef unsigned int uint;
typedef short int int16;
typedef long long int int64;

template<typename T>
struct Vec2
{
	T x;
	T y;

	Vec2()
	{
		x = NULL;
		y = NULL;
	}
	Vec2(T x_i, T y_i)
	{
		x = x_i;
		y = y_i;
	}

	Vec2(Vec2& Input)
	{
		x = Input.x;
		y = Input.y;
	}

	void SetValues(T x_i, T y_i)
	{
		x = x_i;
		y = y_i;
	}

	void SetValues(Vec2& Input)
	{
		x = Input.x;
		y = Input.y;
	}

	bool operator==(const Vec2& other) const
	{
		return false;
	}

};

template<typename T>
struct Vec3
{
	T x;
	T y;
	T z;

	Vec3()
	{
		x = NULL;
		y = NULL;
		z = NULL;
	}
	Vec3(T x_i, T y_i , T z_i)
	{
		x = x_i;
		y = y_i;
		z = z_i;
	}
	Vec3(Vec3& Input)
	{
		x = Input.x;
		y = Input.y;
		z = Input.z;
	}

	void SetValues(T x_i, T y_i, T z_i)
	{
		x = x_i;
		y = y_i;
		z = z_i;
	}

	void SetValues(Vec3& Input)
	{
		this.x = Input.x;
		this.y = Input.y;
		this.z = Input.z;
	}

	bool operator==(const Vec3& other) const
	{
		return false;
	}
};


template<typename T>
struct Vec4
{
	T x;
	T y;
	T z;
	T w;

	Vec4()
	{
		x = NULL;
		y = NULL;
		z = NULL;
		w = NULL;
	}
	Vec4(T x_i, T y_i, T z_i , T w_i)
	{
		x = x_i;
		y = y_i;
		z = z_i;
		w = w_i;
	}
	Vec4(Vec4& Input)
	{
		x = Input.x;
		y = Input.y;
		z = Input.z;
		w = Input.w;
	}

	void SetValues(T x_i, T y_i, T z_i, T w_i)
	{
		x = x_i;
		y = y_i;
		z = z_i;
		w = w_i;
	}

	void SetValues(Vec4& Input)
	{
		x = Input.x;
		y = Input.y;
		z = Input.z;
		w = Input.w;
	}

	bool operator==(const Vec4& other) const
	{
		return false;
	}
};

template<long int size, typename T>
struct Vec_n
{
	T members[size];

	bool operator==(const Vec_n& other) const
	{
		return false;
	}
};

int GiveRandomNumf(int min, int max, int sizeofarray, bool exclude_on_off, int numtoexclude);



