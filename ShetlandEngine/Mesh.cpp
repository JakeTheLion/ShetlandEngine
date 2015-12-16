#include "Mesh.h"
#include "include\SOIL.h"
#include <fstream>
#include <iostream>
#include <string>

/** Default constructor, unused */
Mesh::Mesh(void) {
	vbo = 0;
}

/** Paremeterized constructor, accepts program index */
Mesh::Mesh(GLuint prog, string fileName) {
	programIndex = prog;

	LoadMesh(fileName);
	LoadMaterial(fileName);
	BufferMesh();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // describe position vertex layout to OpenGL)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)sizeofVerts); // describe texture coord layout to OpenGL
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeofVerts + sizeofUVs)); // describe vertex normal layout to OpenGL
	glEnableVertexAttribArray(0); // enable the position attribute
	glEnableVertexAttribArray(1); // enable the texture coordinate attribute
	glEnableVertexAttribArray(2); // enable the normals attribute

	matrix = glGetUniformLocation(programIndex, "worldMatrix"); // get address of the shader matrix variable
}

/** Organizes mesh data (vertices, UVs, and normals) and pushes it into the buffer */
void Mesh::BufferMesh() {
	// create the temporary arrays which we will push to the buffer
	vertices = new GLfloat[numIndices * 3];
	texCoords = new GLfloat[numIndices * 2];
	normals = new GLfloat[numIndices * 3];
	indices = new GLushort[numIndices];

	// variables to control where in the array we're inserting data
	int countedVertices = 0;
	int countedUVs = 0;
	int countedNormals = 0;

	/* Currently working on code to index loaded faces and not reload faces into buffer - not complete/implemented */
	// vector to store faces which have been found for element array
	//vector<vec3> foundFaces = vector<vec3>();

	// loop through the element indices
	for (int i = 0; i < numIndices; ++i) {
		// get the current total face vertex
		vec3 face = indexData[i];

		//int faceDistance = find(foundFaces.begin(), foundFaces.end(), face) - foundFaces.begin();
		//cout << "Distance from start: " << faceDistance << ", NumFaces: " << foundFaces.size() << endl;

		// if the faces we're checking now hasn't already been checked
		//if (faceDistance >= foundFaces.size()) {
			// get the components of that face vertex
			vec3 vertex = vertexData[(GLuint)face[0]];
			vec2 uv = uvData[(GLuint)face[1]];
			vec3 normal = normalData[(GLuint)face[2]];

			// push the vertex data into the temp vertex array
			vertices[countedVertices] = vertex[0];
			vertices[countedVertices + 1] = vertex[1];
			vertices[countedVertices + 2] = vertex[2];
			countedVertices += 3;

			// push the texture coordinate data into the temp uv array
			texCoords[countedUVs] = uv[0];
			texCoords[countedUVs + 1] = uv[1];
			countedUVs += 2;

			// push the normal data into the temp normals array
			normals[countedNormals] = normal[0];
			normals[countedNormals + 1] = normal[1];
			normals[countedNormals + 2] = normal[2];
			countedNormals += 3;

			// an array of increasing integers, basically
			indices[i] = i;

			// add the current face to the list of faces we've used
		//	foundFaces.push_back(face);
		//}
		//else {
		//	indices[i] = (GLushort)faceDistance;
		//}

	}

	// delete previous buffers
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &eao);

	// generate and bind a vertex buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// generate and bind an element array buffer
	glGenBuffers(1, &eao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao);

	// buffer mesh data into the vertex array 
	glBufferData(GL_ARRAY_BUFFER, sizeofVerts + sizeofUVs + sizeofNorms, nullptr, GL_STATIC_DRAW); // buffer an array with the size needed to hold all data
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeofVerts, (void*)vertices); // vertex data
	glBufferSubData(GL_ARRAY_BUFFER, sizeofVerts, sizeofUVs, (void*)texCoords); //  UV data
	glBufferSubData(GL_ARRAY_BUFFER, sizeofVerts + sizeofUVs, sizeofNorms, (void*)normals); //  normals data

	// buffer element index data into the element array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofIndices, indices, GL_STATIC_DRAW);

	// cleanup temp arrays
	delete[] vertices;
	delete[] texCoords;
	delete[] normals;
	delete[] indices;
}

/** Loads a mesh based on its file path */
void Mesh::LoadMesh(string fileName) {
	ifstream inStream;	// create stream object
	string line;			// current line we're reading in
	string file("../Models/" + fileName + ".obj"); // get full file path

	char* token = NULL; // pointer to be used in strtok for token location when splitting strings

	inStream.open(file); // open file to load the data
	if (!inStream.is_open()) { // verify the file opened correctly
		cout << "Error opening file .obj.\n";
		return;
	}

	while (getline(inStream, line)) { // read in file line by line
		switch (line[0]) {
			const char* splitStr; // place to store tokens

		case 'v': // if line stores general vertex data
			switch (line[1]) {
			case ' ': { // if line stores base vertex point data
				++numVerts;
				vec3 vertex = vec3();

				splitStr = strtok_s(&line[0], " v", &token); // read in line split at spaces
				vertex.x = (GLfloat)stof(splitStr); // parse current split element as float
				splitStr = strtok_s(NULL, " v", &token); // read in line split at spaces
				vertex.y = (GLfloat)stof(splitStr); // parse current split element as float
				splitStr = strtok_s(NULL, " v", &token); // read in line split at spaces
				vertex.z = (GLfloat)stof(splitStr); // parse current split element as float

				vertexData.push_back(vertex);
				break;
			}
			case 't': { // if line is a texture coordinate definition
				++numUVs;
				vec2 uv = vec2();

				splitStr = strtok_s(&line[0], " vt", &token); // read in line split at spaces
				uv.x = (GLfloat)stof(splitStr); // parse current split element as float
				splitStr = strtok_s(NULL, " vt", &token); // read in line split at spaces
				uv.y = (GLfloat)stof(splitStr); // parse current split element as float

				uvData.push_back(uv); // parse current split element as float
				break;
			}
			case 'n': { // if line is a texture coordinate definition
				++numNorms;
				vec3 normal = vec3();

				splitStr = strtok_s(&line[0], " vn", &token); // read in line split at spaces
				normal.x = (GLfloat)stof(splitStr); // parse current split element as float
				splitStr = strtok_s(NULL, " vn", &token); // read in line split at spaces
				normal.y = (GLfloat)stof(splitStr); // parse current split element as float
				splitStr = strtok_s(NULL, " vn", &token); // read in line split at spaces
				normal.z = (GLfloat)stof(splitStr); // parse current split element as float

				normalData.push_back(normal); // parse current split element as float
				break;
			}
			}
			break;
		case 'f': { // if line is a face definition
			splitStr = strtok_s(&line[0], " f/", &token); // read in line split at spaces, f and /
			while (splitStr != NULL) { // read each split element
				vec3 face = vec3();
				face[0] = (float)stoi(splitStr) - 1;
				splitStr = strtok_s(NULL, " f/", &token); // get next token
				face[1] = (float)stoi(splitStr) - 1;
				splitStr = strtok_s(NULL, " f/", &token); // get next token
				face[2] = (float)stoi(splitStr) - 1;
				splitStr = strtok_s(NULL, " f/", &token); // get next token

				indexData.push_back(face);
				}
			}
		}
	}

	inStream.close(); // close the stream

	// store number of each element for later when buffering
	numIndices = indexData.size();

	// store size of each element's array for later when buffering
	sizeofIndices = numIndices * sizeof(GLushort);
	sizeofVerts = numIndices * 3 * sizeof(GLfloat);
	sizeofUVs = numIndices * 2 * sizeof(GLfloat);
	sizeofNorms = sizeofVerts;

	cout << "Model " << fileName << " loaded.\n";
}

/** Load a material file based on the model */
void Mesh::LoadMaterial(string fileName)
{
	ifstream inStream;	// create stream object
	string line;		// current line we're reading in
	string file("../Models/" + fileName + ".mtl"); // get full file path to materials

	char* token = NULL; // pointer to be used in strtok for token location when splitting strings
	string texture = ""; // string name of texture file to be loaded in by SOIL

	inStream.open(file); // open file to load the data
	if (!inStream.is_open()) { // verify the file opened correctly
		cout << "Error opening material .mtl file " << fileName << " .mtl\n";
		return;
	}

	// read in file line by line
	while (getline(inStream, line)) { 
		if (line[0] == 'm' && line[1] == 'a' && line[2] == 'p') { // if line holds the texture file data
			texture = "../Models/" + (line.substr(line.find(" ") + 1));
		}
	}
	inStream.close(); // close the stream
	cout << "Material name is " << texture << ", retrieved from file " << fileName << ".mtl\n";

	// load the texture into a new ID with SOIL
	textureID = SOIL_load_OGL_texture(	
		&texture[0], SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	// check if the texture loaded correctly
	if (textureID == 0) {
		cout << "SOIL error loading texture " << texture << endl;
	}

	// bind the texture in OpenGL
	glBindTexture(GL_TEXTURE_2D, textureID); 
}

/** Render overload to render with rotation */
void Mesh::Render(vec3 drawPos, vec3 drawScale, vec3 drawRotationAxis, float rotation) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao);

	// send world matrix to vertex shader
	glProgramUniformMatrix4fv(programIndex, matrix, 1, false, &(translate(mat4(1.0f), drawPos) * rotate(mat4(1.0f), rotation, drawRotationAxis) * scale(mat4(1.0f), drawScale))[0][0]);

	// push active buffer to render
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
}

/** Destructor, deletes vertex objects (currently disabled for bugs) */
Mesh::~Mesh(void) {
	// delete buffers
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &eao);

	cout << "Destructor\n";
}