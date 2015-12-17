#include "ShaderHelpers.h"
#include <iostream>
#include <fstream>
using namespace std;

/** Default constructor, unused */
ShaderHelpers::ShaderHelpers(void)
{
}

/** Loads a shader to a string */
/**
	Creates and returns a c-string of a shader loaded in from a file.

	@file a c-string representing the file path to the shader.
*/
char* ShaderHelpers::loadTextFile(const char* file) {
	ifstream inStream(file, ios::binary);			// open stream from shader file
	inStream.seekg(0, ios::end);					// seek to the end of the shader
	const int length = (int)inStream.tellg();		// store file length
	inStream.seekg(0, ios::beg);					// seek back to the beginning

	char* fileContents = new char[length + 1];		// array to store shader string in
	if (inStream.is_open())							// make sure the stream opened correctly
	{
		inStream.read(fileContents, length);		// read from file if it's open
		fileContents[length] = 0;					// null terminator
	}
	else
	{
		return 0;									// return if the stream didn't open correctly
	}

	inStream.close();								// close the in stream
	return fileContents;
}

/** Create a shader */
/**
	Create a shader and return its index so it can be attached to a program.

	@file a c-string representing the path to the shader
	@shaderType what type of shader we are loading (vertext, fragment, etc.)
*/
GLuint ShaderHelpers::loadShader(const char* file, GLenum shaderType) {
	const char* shaderFile = loadTextFile(file);								// get the file as a string
	
	if (shaderFile == 0)														// return and print error if file loaded incorrectly
	{
		cout << "\nError loading shader file '" << file << "'.";				
		return 0;
	}

	GLuint shader = glCreateShader(shaderType);									// create a shader
	glShaderSource(shader, 1, (const GLchar**)&shaderFile, 0);					// set source to loaded file
	glCompileShader(shader);													// compile the shader
	delete shaderFile;

	GLint* compiled = new GLint();
	glGetShaderiv(shader, GL_COMPILE_STATUS, (GLint*)compiled);					// get compile status of shader

	if (compiled)
		return shader;															// return shader if it compiled
	else																		// didn't compile, return info log
	{
		const GLint* logLength = new GLint();
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, (GLint*)logLength);			// store length of log

		const char* infoLog = new char[*logLength + 1];
		glGetShaderInfoLog(shader, (GLsizei)logLength, 0, (GLchar*)infoLog);	// store log in string

		cout << "\n" << infoLog;												// print log and delete unneeded vars
		glDeleteShader(shader);
		delete infoLog;
		delete compiled;
		return 0;
	}
}

/** Creates a shader program */
/**
	Loads vertex and fragment shaders, attaches them to a program, and returns the program's index.

	@vertexFile a string representing the file path to the vertex shader
	@fragmentFile a string representing the file path to the fragment shader
*/
GLuint ShaderHelpers::loadShaderProgram(const char* vertexFile, const char* fragmentFile) {
	GLuint vertexProgram = loadShader(vertexFile, GL_VERTEX_SHADER);				// load the vertext shader program
	if (vertexProgram == 0)															// return and print error if program loaded incorrectly
	{
		cout << "/nError loading vertex shader program '" << vertexFile << "'.";
		return 0;
	}
	GLuint fragmentProgram = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (fragmentProgram == 0)														// return and print error if program loaded incorrectly
	{
		cout << "/nError loading fragment shader program '" << vertexFile << "'.";
		return 0;
	}

	GLuint programIndex = glCreateProgram();										// create shader program
	glAttachShader(programIndex, vertexProgram);									// attach the shaders
	glAttachShader(programIndex, fragmentProgram);
	glLinkProgram(programIndex);													// link the program
	
	GLint* compiled = new GLint();
	glGetShaderiv(programIndex, GL_LINK_STATUS, compiled);							// get link status of shader program

	if (compiled)
		return programIndex;														// return program if it linked
	else																			// didn't link correctly, return info log
	{
		GLint* logLength = new GLint();
		glGetShaderiv(programIndex, GL_INFO_LOG_LENGTH, logLength);					// store length of log

		const char* infoLog = new char[*logLength + 1];
		glGetShaderInfoLog(programIndex, (GLsizei)logLength, 0, (GLchar*)infoLog);	// store log in string

		cout << "\n" << infoLog;													// print log and delete unneeded vars
		glDeleteProgram(programIndex);
		delete logLength;
		delete infoLog;
		delete compiled;
		return 0;
	}
}

/** Sets the current color of the shader */
/**
	Passes a color into a uniform shader variable.

	@programIndex a GLuint ID of the program
	@uniformVar a c-string name of the uniform color variable
	@red the r value of the color
	@green the g value of the color
	@blue the b value of the color
*/
void ShaderHelpers::setShaderColor(GLuint programIndex, const char* uniformVar, float red, float green, float blue) {
	GLuint variableAddress = glGetUniformLocation(programIndex, uniformVar); // get address of the shader color variable
	glProgramUniform4f(programIndex, variableAddress, red, green, blue, 1.0f); // set color of variable to parameters
}

/** Sets the current color of the shader using a vec3 */
/**
	Passes a color into a uniform shader variable using a vec3 to represent color instead of rgb.

	@programIndex a GLuint ID of the program
	@uniformVar a c-string name of the uniform color variable
	@color a 3 dimensional vector where xyz matches to rgb
*/
void ShaderHelpers::setShaderColor(GLuint programIndex, const char* uniformVar, vec3 color) {
	GLuint variableAddress = glGetUniformLocation(programIndex, uniformVar); // get address of the shader color variable
	glProgramUniform4f(programIndex, variableAddress, color.x, color.y, color.z, 1.0f); // set color of variable to parameters
}

/** Sets global position offset of the shader */
void ShaderHelpers::setShaderVec2(GLuint programIndex, const char* uniformVar, vec2 vector) {
	GLuint variableAddress = glGetUniformLocation(programIndex, uniformVar); // get address of the shader offset variable
	glProgramUniform2fv(programIndex, variableAddress, 1, &vector[0]); // send data to the shader address
}

ShaderHelpers::~ShaderHelpers(void)
{
}
