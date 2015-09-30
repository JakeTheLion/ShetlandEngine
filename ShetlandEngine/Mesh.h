#include <glew.h>
#include <glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <vector>
using namespace std;
using namespace glm;
#pragma once

/** Mesh Class, currently unused */
class Mesh
{
	public:
		Mesh(void);
		Mesh(GLuint prog);
		void LoadMesh(string fileName); // load a .obj file into the model's vector members
		void LoadMaterial(string fileName); // load a .mtl file's data to prepare for texturing
		void Render(vec3 drawPos, vec3 drawScale); // render the mesh
		void Render(vec3 drawPos, vec3 drawScale, vec3 drawRotationAxis, float rotation); // render the shape with rotation
		~Mesh(void);

	private:
		vector<GLfloat> vertices;	// array to hold the vertex data of the mesh
		vector<GLfloat> normals;	// array to hold the vertex data of the mesh
		vector<GLfloat> texCoords;	// array to hold the vertex data of the mesh
		vector<GLushort> indices;	// array to hold elements of mesh in order
		GLuint vbo, vao, eao;		// holds the mesh's vertex buffer and arrays and element array
		GLuint programIndex;		// holds program index
		GLint matrix;				// location of matrix shader variable
		int numVerts, numIndices;	// number of vertices in the mesh
		string textureFile;			// string storing name of mesh's texture file, loaded from the obj's .mtl
};