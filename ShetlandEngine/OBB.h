#include <vector>
#include "Quaternion.h"
using namespace glm;
using namespace std;
#pragma once

class OBB
{
	public:
		// Constructors
		OBB();							// Default
		OBB(vec3 _c, vec3 _e);			// Parameterized - builds OBB from passed center and half width
		OBB(vector<vec3>, vec3 c);		// Parameterized - builds OBB from vector of vec3s representing a mesh
		
		// Transformers
		void Rotate(vec3 axis, float radians);
		void Scale(float scalar);
		bool collidesWith(OBB&);

		// Data
		vec3 center;	// Center of the OBB
		vec3 u[3];		// The OBB's axes in world space
		vec3 e;		// Half width of OBB in all three dimensions

		// Destructor
		~OBB();

	private:
};

