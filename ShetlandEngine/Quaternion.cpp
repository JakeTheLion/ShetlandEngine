#include "Quaternion.h"
#include <math.h>
#include <iostream>
using namespace std;

//=== DEFAULT CONSTRUCTOR ===//
Quaternion::Quaternion()
{
	vector = vec3();
	myW = 0.0f;
}

//=== PARAMETERIZED CONSTRUCTOR ===//
Quaternion::Quaternion(float d0, float d1, float d2)
{
	vector = vec3(d0, d1, d2);
	myW = 0.0f;
}

//=== PARAMETERIZED CONSTRUCTOR ===//
Quaternion::Quaternion(float _w, vec3 _vector)
{
	vector = _vector;
	myW = _w;
}

//=== SLERP ===//
// Returns quaternion slerpVal % lerped from q1 to q2
Quaternion Quaternion::slerp(Quaternion q1, Quaternion q2, float slerpVal)
{
	return ((q2 * q1^-1)^slerpVal) * q1;
}

//=== POINT ===//
// Returns a quaternion representing a point
Quaternion Quaternion::point(float x, float y, float z)
{
	// new point quaternion
	return Quaternion(x, y, z);
}

//=== ROTATION ===//
// Returns a quaternion representing a rotation
Quaternion Quaternion::rotation(float theta, vec3 rotationVector)
{
	// new rotation quaternion
	return Quaternion(cosf(theta / 2), rotationVector*sin(theta / 2));
}

//=== MULTIPLICATION OPERATOR ===//
// Returns multiplication of the two quaternions
Quaternion Quaternion::operator*(Quaternion other)
{
	// construct the new quaternion
	Quaternion q = Quaternion();
	q.myW = (w()*other.w()) - (dot(v(), other.v()));
	q.vector = (other.v()*w()) + (v()*other.w()) + (cross(v(), other.v()));

	return q;
}

//=== MULTIPLICATION OPERATOR ===//
// Returns the vector rotated by the Quaternion
vec3 Quaternion::rotate(Quaternion q, vec3 other)
{
	vec3 v = q.v() * dot(q.v(), other) * 2.0f
		   + other * (q.w()*q.w() - dot(q.v(), q.v()))
		   + cross(q.v(), other) * 2.0f * q.w();

	return v;
}

//=== EXPONENT OPERATOR ===//
// Returns this raised to the given power
Quaternion Quaternion::operator^(float power)
{
	// New empty quaternion
	Quaternion q = Quaternion();
	q.vector = axis()*sinf(power * theta() / 2);
	q.myW = cosf(power * theta() / 2);

	return q;
}

//=== ACCESSOR: theta ===//
// Returns the theta of the quaternion
float Quaternion::theta()
{
	return 2 * acosf(myW);
}

//=== ACCESSOR: axis ===//
// Returns the quaternion's rotation axis
vec3 Quaternion::axis()
{
	return v() / sin(theta() / 2.0f);
}

//=== DESTRUCTOR ===//
Quaternion::~Quaternion()
{
}
