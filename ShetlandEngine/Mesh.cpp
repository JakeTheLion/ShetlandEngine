#include "Mesh.h"
#include "include\SOIL.h"
#include <fstream>
#include <iostream>
#include <string>

/** Default constructor, unused */
Mesh::Mesh(void) {
	vao = 0;
	vbo = 0;
}
/** Paremeterized constructor, accepts program index */
Mesh::Mesh(GLuint prog) {
	programIndex = prog;

	glGenVertexArrays(1, &vao);	// generate a vertex array
	glBindVertexArray(vao); // bind vertex array
	
	glGenBuffers(1, &vbo); // generate a vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind vertex buffer

	glGenBuffers(1, &eao); // generate element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao); // bind element buffer

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (sizeof(GLfloat)*3), 0); // describe position vertex layout to OpenGL
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (sizeof(float)*5), (void*)(sizeof(float)*2)); // describe color vertex layout to OpenGL
	glEnableVertexAttribArray(0); // enable the position attribute
	//glEnableVertexAttribArray(1); // enable the color attribute

	matrix = glGetUniformLocation(programIndex, "worldMatrix"); // get address of the shader matrix variable
	cout << "Constructor done\n";
}

/** Loads a mesh based on its file path */
void Mesh::LoadMesh(string fileName) {
	ifstream inStream;	// create stream object
	string line;			// current line we're reading in
	string file("../Models/" + fileName + ".obj"); // get full file path

	char* token = NULL; // pointer to be used in strtok for token location when splitting strings

	inStream.open(file); // open file to load the data
	if (inStream.is_open()) { // verify the file opened correctly
		while (getline(inStream, line)) { // read in file line by line
			if (strlen(&line[0]) > 2) { // only attempt to read lines with vertex/face data
				if (line[0] == 'v' && line[1] == ' ') { // if line is a vertex definition
					const char* splitStr = strtok_s(&line[0], " v", &token); // read in line split at spaces

					while (splitStr != NULL) { // read each split element
						vertices.push_back((GLfloat)stof(splitStr)); // parse current split element as float
						splitStr = strtok_s(NULL, " v", &token); // get next token
					}
				}
				if (line[0] == 'f') { // if line is a face definition
					const char* splitStr = strtok_s(&line[0], " f/", &token); // read in line split at spaces, f and /
					while (splitStr != NULL) { // read each split element

						indices.push_back(stoi(splitStr)-1);
						splitStr = strtok_s(NULL, " f/", &token); // get next token
						splitStr = strtok_s(NULL, " f/", &token); // get next token
						splitStr = strtok_s(NULL, " f/", &token); // get next token
					}
				}
			}
		}

		inStream.close(); // close the stream
	
		numVerts = vertices.size(); // store number of for later when buffering mesh
		numIndices = indices.size(); // store number of indices for later when buffering element array
		
		cout << "Model loaded\n";
	}
	else {
		cout << "Error opening file .obj.\n";
		return;
	}
}

/** Load a material file based on the model */
void Mesh::LoadMaterial(string fileName)
{
	ifstream inStream;	// create stream object
	string line;		// current line we're reading in
	string file("../Models/" + fileName + ".mtl"); // get full file path to materials

	char* token = NULL; // pointer to be used in strtok for token location when splitting strings

	inStream.open(file); // open file to load the data
	if (inStream.is_open()) { // verify the file opened correctly
		while (getline(inStream, line)) { // read in file line by line
			if (strlen(&line[0]) > 2) { // only attempt to read lines with vertex/face data
				if (line[0] == 'm' && line[1] == 'a' && line[2] == 'p') { // if line holds the texture file daya
					const char* splitStr = strtok_s(&line[0], " ", &token); // read in line split at spaces

					while (splitStr != NULL) { // read each split element
						vertices.push_back((GLfloat)stof(splitStr)); // parse current split element as float
						splitStr = strtok_s(NULL, " v", &token); // get next token
					}
				}
			}
		}

		inStream.close(); // close the stream

		numVerts = vertices.size(); // store number of for later when buffering mesh
		numIndices = indices.size(); // store number of indices for later when buffering element array

		cout << "Model loaded\n";
	}
	else {
		cout << "Error opening file .obj.\n";
		return;
	}
}

/** Base render, just scale and position */
void Mesh::Render(vec3 drawPos, vec3 drawScale)	 {
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numVerts, &vertices[0], GL_STATIC_DRAW); // copy vertex data into the active buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*numIndices, &indices[0], GL_STATIC_DRAW);
	glProgramUniformMatrix4fv(programIndex, matrix, 1, false, &(translate(mat4(1.0f), drawPos) * scale(mat4(1.0f), drawScale))[0][0]); // send world matrix to vertex shader
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0); // push active buffer to render
}

/** Render overload to render with rotation */
void Mesh::Render(vec3 drawPos, vec3 drawScale, vec3 drawRotationAxis, float rotation) {
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numVerts, &vertices[0], GL_STATIC_DRAW); // copy vertex data into the active buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*numIndices, &indices[0], GL_STATIC_DRAW);
	glProgramUniformMatrix4fv(programIndex, matrix, 1, false, &(translate(mat4(1.0f), drawPos) * rotate(mat4(1.0f), rotation, drawRotationAxis) * scale(mat4(1.0f), drawScale))[0][0]); // send world matrix to vertex shader
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0); // push active buffer to render
}

/** Destructor, deletes vertex objects (currently disabled for bugs) */
Mesh::~Mesh(void) {
	// delete the vertex objects
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	// delete the mesh
	cout << "Destructor\n";
}