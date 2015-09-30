#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
using namespace glm;

/** Shader Helper */
// Helper methods to load in shader files and send
// information to uniform shader variables
class ShaderHelpers
{
	public:
		ShaderHelpers(void);
		static char* loadTextFile(const char* file); // loads a shader file to a string
		static GLuint loadShader(const char* file, GLenum shaderType); // loads string into shader
		static GLuint loadShaderProgram(const char* vertextFile, const char* fragmentFile); // initiates shader as program
		static void setShaderColor(GLuint programIndex, const char* uniformVar, float red, float green, float blue); // sets color of a shader
		static void setShaderColor(GLuint programIndex, const char* uniformVar, vec3 color); // sets color of a shader with vec3
		static void setShaderVec2(GLuint programIndex, const char* uniformVar, glm::vec2 vector);
		~ShaderHelpers(void);
};