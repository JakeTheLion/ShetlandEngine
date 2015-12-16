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
		Mesh(GLuint prog, string fileName);
		void BufferMesh();
		void LoadMesh(string fileName); // load a .obj file into the model's vector members
		void LoadMaterial(string fileName); // load a .mtl file's data to prepare for texturing
		void Render(vec3 drawPos, vec3 drawScale, vec3 drawRotationAxis, float rotation); // render the shape with rotation
		~Mesh(void);

	private:
		vector<vec3> vertexData;		// array to hold the vertex data of the mesh
		vector<vec3> normalData;		// array to hold the vertex data of the mesh
		vector<vec2> uvData;			// array to hold the vertex data of the mesh
		vector<vec3> indexData;			// array to hold indices elements of mesh in order
		GLfloat* vertices;				// array to hold the vertex data while it's transferred into a buffer for drawing
		GLfloat* normals;				// array to hold the normal data while it's transferred into a buffer for drawing
		GLfloat* texCoords;				// array to hold the tex/uv data while it's transferred into a buffer for drawing
		GLushort* indices;				// array to hold the data indices while it's transferred into a buffer for drawing
		GLuint vbo, eao;				// holds the mesh's vertex buffer and arrays and element array
		GLuint programIndex;			// holds program index
		GLuint textureID;				// SOIL ID of mesh's texture
		GLint matrix;					// location of matrix shader variable
		string textureFile;				// string storing name of mesh's texture file, loaded from the obj's .mtl
		int numVerts, numNorms, numUVs, numIndices;	// number of each type of element in the mesh
		int sizeofVerts, sizeofNorms, sizeofUVs, sizeofIndices; // number of bytes taken up by each set of info
};