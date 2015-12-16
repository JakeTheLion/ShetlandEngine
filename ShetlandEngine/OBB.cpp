#include "OBB.h"

/// Default Ctor
// Set default values for all variables
OBB::OBB()
{
	// XYZ axes
	u[0] = vec3(1.0f, 0.0f, 0.0f);
	u[1] = vec3(0.0f, 1.0f, 0.0f);
	u[2] = vec3(0.0f, 0.0f, 1.0f);

	// Center
	center = vec3();

	// Basic halfwidths
	e[0] = e[1] = e[2] = 0.5f;
}

/// Parameterized Ctor - builds OBB from passed center and half width
OBB::OBB(vec3 _c, vec3 _e)
{
	// Default XYZ axes
	u[0] = vec3(1.0f, 0.0f, 0.0f);
	u[1] = vec3(0.0f, 1.0f, 0.0f);
	u[2] = vec3(0.0f, 0.0f, 1.0f);

	center = _c;
	e = _e;
}

/// Parameterized Ctor - Builds OBB from vec3s representing a mesh
OBB::OBB(vector<vec3> mesh, vec3 c) {
	// XYZ axes
	u[0] = vec3(1.0f, 0.0f, 0.0f);
	u[1] = vec3(0.0f, 1.0f, 0.0f);
	u[2] = vec3(0.0f, 0.0f, 1.0f);

	// Initial center
	center = c;

	// Find the initial lower/upper from the first two. Saves accidental
	// errors with wrong coords e.g. model is entirely in negative,
	// vec3s init at 0, upper is set to 0,0,0 even though it isn't
	vec3 lowerAABB = vec3();
	vec3 upperAABB = vec3();
	lowerAABB.x = min(mesh[0].x, mesh[1].x); upperAABB.x = max(mesh[0].x, mesh[1].x);
	lowerAABB.y = min(mesh[0].y, mesh[1].y); upperAABB.y = max(mesh[0].y, mesh[1].y);
	lowerAABB.z = min(mesh[0].z, mesh[1].z); upperAABB.z = max(mesh[0].z, mesh[1].z);

	// Loop through the mesh array
	for (int i = 2; i < mesh.size(); ++i) {
		// Compare with current max/min values
		if (mesh[i].x < lowerAABB.x) lowerAABB.x = mesh[i].x;
		if (mesh[i].y < lowerAABB.y) lowerAABB.y = mesh[i].y;
		if (mesh[i].z < lowerAABB.z) lowerAABB.z = mesh[i].z;
		if (mesh[i].x > upperAABB.x) upperAABB.x = mesh[i].x;
		if (mesh[i].y > upperAABB.y) upperAABB.y = mesh[i].y;
		if (mesh[i].z > upperAABB.z) upperAABB.z = mesh[i].z;
	}

	// Assign halfwidths based on found bounds
	e[0] = abs(upperAABB.x - lowerAABB.x) / 2;
	e[1] = abs(upperAABB.y - lowerAABB.y) / 2;
	e[2] = abs(upperAABB.z - lowerAABB.z) / 2;

	// Initial half-widths
	u[0] = vec3(1.0f, 0.0f, 0.0f);
	u[1] = vec3(0.0f, 1.0f, 0.0f);
	u[2] = vec3(0.0f, 0.0f, 1.0f);
}

/// Rotate()
// Rotates the OBB
void OBB::Rotate(vec3 axis, float radians) {
	// Quaternion that will apply the rotation
	Quaternion rotation = Quaternion::rotation(radians, axis);

	// Rotate both bounds of the OBB by the quaternion
	u[0] = Quaternion::rotate(rotation, u[0]);
	u[1] = Quaternion::rotate(rotation, u[1]);
	u[2] = Quaternion::rotate(rotation, u[2]);
}

/// Scale()
// Scales the model (multiplies all parts by a float)
// Not used, but Cyprian mentioned it so \o/
void OBB::Scale(float scalar) {
	// Scale down components of both upper/lowerOBB
	e[0] *= scalar;
	e[1] *= scalar;
	e[2] *= scalar;
}

/// Checks for collisions against the passed OBB
// @b	The other OBB to check against
bool OBB::collidesWith(OBB& b)
{
	// Using the method from Real-time Collision Detection book
	vec3 T = b.center - center;
	mat3 R = mat3();
	vec3 L;
	int x = 0; int y = 1; int z = 2; // just for help

									 // Create a matrix of reused precalculated dot products of each a[u] b[u] combination
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			R[i][j] = dot(u[i], b.u[j]);
		}

	// Test some axes
	for (int i = 0; i < 3; i++) {
		// Test when L is one of A's axes
		L = u[i];
		if (abs(dot(T, L)) > e[i] + abs(b.e[x] * R[i][x]) + abs(b.e[y] * R[i][y]) + abs(b.e[z] * R[i][z])) return 0;
	}

	for (int i = 0; i < 3; i++) {
		// Test when L is one of B's axes
		L = b.u[i];
		if (abs(dot(T, L)) > abs(e[x] * R[x][i]) * abs(e[y] * R[y][i]) * abs(e[z] * R[z][i]) + b.e[i]) return 0;
	}

	// Test u[x] x b.u[x]
	L = cross(u[x], b.u[x]);
	if (abs(dot(T, L)) > (abs(e[y] * R[z][x]) + abs(e[z] * R[y][x]) + abs(b.e[y] * R[x][z]) + abs(b.e[z] * R[x][y]))) return 0;
	// Test u[x] x b.u[y]
	L = cross(u[x], b.u[y]);
	if (abs(dot(T, L)) > (abs(e[y] * R[z][y]) + abs(e[z] * R[y][y]) + abs(b.e[x] * R[x][z]) + abs(b.e[z] * R[x][x]))) return 0;
	// Test u[x] x b.u[z]
	L = cross(u[x], b.u[z]);
	if (abs(dot(T, L)) > (abs(e[y] * R[z][z]) + abs(e[z] * R[y][z]) + abs(b.e[x] * R[x][y]) + abs(b.e[y] * R[x][x]))) return 0;


	// Test u[y] x b.u[x]
	L = cross(u[y], b.u[x]);
	if (abs(dot(T, L)) > (abs(e[x] * R[z][x]) + abs(e[z] * R[x][x]) + abs(b.e[y] * R[y][z]) + abs(b.e[z] * R[y][y]))) return 0;
	// Test u[y] x b.u[y]
	L = cross(u[y], b.u[y]);
	if (abs(dot(T, L)) > (abs(e[x] * R[z][y]) + abs(e[z] * R[x][y]) + abs(b.e[x] * R[y][z]) + abs(b.e[z] * R[y][x]))) return 0;
	// Test u[y] x b.u[z]
	L = cross(u[y], b.u[z]);
	if (abs(dot(T, L)) > (abs(e[x] * R[z][z]) + abs(e[z] * R[x][z]) + abs(b.e[x] * R[y][y]) + abs(b.e[y] * R[y][x]))) return 0;

	// Test u[z] x b.u[x]
	L = cross(u[z], b.u[x]);
	if (abs(dot(T, L)) > (abs(e[x] * R[y][x]) + abs(e[y] * R[x][x]) + abs(b.e[y] * R[z][z]) + abs(b.e[z] * R[z][y]))) return 0;
	// Test u[z] x b.u[y]
	L = cross(u[z], b.u[y]);
	if (abs(dot(T, L)) > (abs(e[x] * R[y][y]) + abs(e[y] * R[x][y]) + abs(b.e[x] * R[z][z]) + abs(b.e[z] * R[z][x]))) return 0;
	// Test u[z] x b.u[z]
	L = cross(u[z], b.u[z]);
	if (abs(dot(T, L)) > (abs(e[x] * R[y][z]) + abs(e[y] * R[x][z]) + abs(b.e[x] * R[z][y]) + abs(b.e[y] * R[z][x]))) return 0;

	// No separating axis found -- collision!
	return 1;
}

OBB::~OBB()
{
}
