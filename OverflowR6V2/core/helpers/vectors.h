#pragma once
#include <cmath>
#include <algorithm>

#define M_PI	3.14159265358979323846264338327950288419716939937510

//vec_t
class vec_t
{
public:
	vec_t() : x(0.f), y(0.f)
	{

	}

	vec_t(float _x, float _y) : x(_x), y(_y)
	{

	}
	~vec_t()
	{

	}

	float x;
	float y;
};

//vec3_t
class vec3_t
{
public:
	vec3_t() : x(0.f), y(0.f), z(0.f)
	{

	}

	vec3_t(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~vec3_t()
	{

	}

	float x;
	float y;
	float z;

	inline float Dot(vec3_t v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(vec3_t v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}
	inline float Length()
	{
		float ls = x * x + y * y + z * z;
		return sqrt(ls);
	}

	vec3_t operator+(vec3_t v)
	{
		return vec3_t(x + v.x, y + v.y, z + v.z);
	}

	vec3_t operator-(vec3_t v)
	{
		return vec3_t(x - v.x, y - v.y, z - v.z);
	}

	vec3_t operator*(float number) const {
		return vec3_t(x * number, y * number, z * number);
	}

	vec3_t& operator-=(const vec3_t& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}


	void clamp()
	{
		if (x > 75.f) x = 75.f;
		else if (x < -75.f) x = -75.f;
		if (z < -180) z += 360.0f;
		else if (z > 180) z -= 360.0f;

		y = 0.f;
	}

};

//vec4_t
class vec4_t
{
public:
	vec4_t() : x(0.f), y(0.f), z(0.f), w(0.f)
	{

	}

	vec4_t(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{

	}
	~vec4_t()
	{

	}

	float x;
	float y;
	float z;
	float w;
};