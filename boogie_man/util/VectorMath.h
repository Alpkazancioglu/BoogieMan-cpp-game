#pragma once
#include <random>
#include <time.h>
#include <iostream>
#include <string>
#include <raylib.h>


//Utility functions and definitions//>
#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

typedef unsigned int uint;
typedef short int int16;
typedef long long int int64;

float Q_rsqrt(float number);
int GiveRandomNumf(int min, int max, int sizeofarray, bool exclude_on_off, int numtoexclude);
// // // // // // // // // // // // //<

/// <summary>
/// Math class that describes a 2D vector with complete set of parameters and functions
/// </summary>
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
	Vec2(const T& x, const T& y)
		: x(x), y(y)
	{
	}
	Vec2(const Vec2& Input)
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

	void InverseSQroot()
	{
		x = Q_rsqrt(x);
		y = Q_rsqrt(y);
	}

	Vector2 toVector2()
	{
		return { this->x , this->y };
	}

	bool operator==(const Vec2& other) const
	{
		return this->x == other.x && this->y == other.y;
	}

	Vec2 operator+(const Vec2& other) const
	{
		return { x + other.x , y + other.y };
	}

	Vec2 operator+(const T& scalar) const
	{
		return { x + scalar, y + scalar };
	}

	Vec2 operator-(const Vec2& other) const
	{
		return { x - other.x , y - other.y };
	}

	Vec2 operator-(const T& scalar) const
	{
		return { x - scalar, y - scalar };
	}

	Vec2 operator*(const T& scalar) const
	{
		return { x * scalar, y * scalar };
	}

	Vec2 operator/(const T& scalar) const
	{
		return { x / scalar, y / scalar };
	}

	Vec2 operator/(const Vec2& other) const
	{
		return { x / other.x, y / other.y };
	}

	Vec2 operator*(const Vec2& other) const
	{
		return { x * other.x, y * other.y };
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec2& vec)
	{
		os << "(" << vec.x << ", " << vec.y << ")";
		return os;
	}

	void operator()(const Vec2& other) const
	{
		this->x = other.x;
		this->y = other.y;
	}

};

/// <summary>
/// Math class that describes a 3D vector with complete set of parameters and functions
/// </summary>
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
	Vec3(const T& x, const T& y, const T& z)
		: x(x), y(y), z(z)
	{
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

	void InverseSQroot()
	{
		x = Q_rsqrt(x);
		y = Q_rsqrt(y);
		z = Q_rsqrt(z);
	}

	Vector3 toVector3()
	{
		return { this->x , this->y , this->z };
	}

	bool operator==(const Vec3& other) const
	{
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}

	Vec3 operator+(const Vec3& other) const
	{
		return { x + other.x , y + other.y , z + other.z};
	}

	Vec3 operator+(const T& scalar) const
	{
		return { x + scalar, y + scalar, z + scalar };
	}

	Vec3 operator-(const Vec3& other) const
	{
		return { x - other.x , y - other.y , z - other.z };
	}

	Vec3 operator-(const T& scalar) const
	{
		return { x - scalar, y - scalar, z - scalar };
	}

	Vec3 operator*(const T& scalar) const
	{
		return { x * scalar, y * scalar, z * scalar };
	}

	Vec3 operator/(const T& scalar) const
	{
		return { x / scalar, y / scalar, z / scalar };
	}

	Vec3 operator*(const Vec3& other) const
	{
		return { x * other.x, y * other.y, z * other.z };
	}

	Vec3 operator/(const Vec3& other) const
	{
		return { x / other.x, y / other.y, z / other.z };
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec3& vec)
	{
		os << "(" << vec.x << ", " << vec.y << ", "<< vec.z << ")";
		return os;
	}

	void operator()(const Vec3& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}
	
};


/// <summary>
/// Math class that describes a 4D vector with complete set of parameters and functions
/// </summary>
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

	Vec4(const T& x, const T& y, const T& z, const T& w)
		: x(x), y(y), z(z), w(w)
	{
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

	Vector4 toVector4()
	{
		return { this->x , this->y , this->z  , this->w};
	}

	void InverseSQroot()
	{
		x = Q_rsqrt(x);
		y = Q_rsqrt(y);
		z = Q_rsqrt(z);
		w = Q_rsqrt(w);
	}

	bool operator==(const Vec4& other) const
	{
		return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w;
	}

	Vec4 operator+(const Vec4& other) const
	{
		return { x + other.x , y + other.y , z + other.z  , w + other.w };
	}

	Vec4 operator+(const T& scalar) const
	{
		return { x + scalar, y + scalar, z + scalar, w + scalar };
	}

	Vec4 operator-(const Vec4& other) const
	{
		return { x - other.x , y - other.y , z - other.z  , w - other.w };
	}

	Vec4 operator-(const T& scalar) const
	{
		return { x - scalar, y - scalar, z - scalar, w - scalar };
	}

	Vec4 operator*(const T& scalar) const
	{
		return { x * scalar, y * scalar, z * scalar, w * scalar };
	}

	Vec4 operator/(const T& scalar) const
	{
		return { x / scalar, y / scalar, z / scalar, w / scalar };
	}

	Vec4 operator/(const Vec4& other) const
	{
		return { x / other.x, y / other.y, z / other.z, w / other.w };
	}

	Vec4 operator*(const Vec4& other) const
	{
		return { x * other.x, y * other.y, z * other.z, w * other.w };
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec4& vec)
	{
		os << "(" << vec.x << ", " << vec.y << ", "<< vec.z << ", "<<vec.w << ")";
		return os;
	}

	void operator()(const Vec4& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
	}

};

template<long int size, typename T>
struct Vec_n
{
	T members[size];

	bool operator==(const Vec_n& other) const
	{
		bool result = true;

		for (size_t i = 0; i < size; i++)
		{
			result = this->members[i] == other[i] && result;
			if (!result)
			{
				break;
			}
		}

		return result;
	}
};




