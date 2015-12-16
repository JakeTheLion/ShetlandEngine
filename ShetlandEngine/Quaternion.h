#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
using namespace glm;

class Quaternion
{
	public:
		// constructors and destructors
		Quaternion();
		Quaternion(float x, float y, float z);
		Quaternion(float _w, vec3 _vector);
		~Quaternion();

		// static Quaternion creation functions
		static Quaternion slerp(Quaternion q1, Quaternion q2, float slerpVal);
		static Quaternion point(float x, float y, float z);
		static Quaternion rotation(float theta, vec3 rotationVector);
		static vec3 rotate(Quaternion q, vec3 v);

		// operator overloads
		Quaternion operator*(Quaternion other);	// multiplies Quaternions
		Quaternion operator^(float power);		// raises the Quaternion to a power

		// accessors
		float theta();	// returns Quaternion's theta
		vec3 axis();	// returns Quaternion's rotation axis

		// Quaternion component accessors
		vec3 v() {
			return vector;
		};
		float w() {
			return myW;
		};

	private:
		vec3 vector;
		float myW;
};

