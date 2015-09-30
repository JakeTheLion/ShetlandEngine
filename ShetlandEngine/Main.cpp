#include <glew.h>
#include <glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include "ShaderHelpers.h"
#include "GameObject.h"
#include <time.h>
using namespace std;
using namespace glm;

/** Global Variables */
ShaderHelpers shaderHelper = ShaderHelpers();
GLuint programIndex;
Mesh* mesh; // create meshes
const int NUM_OBJECTS = 100; // max number of game objects
GameObject gameObjects[NUM_OBJECTS]; // game objects, empty by default
vec2 mousePos = vec2(0.0f);
vec2 prevMousePos = vec2(0.0f);
float currentTime, prevTime, deltaTime; // time control vars for object physics
/* Create an OpenGL window context */
GLFWwindow* windowPtr;

/** Main initialisation for shaders and shapes */
void init() {
	srand((unsigned int)time(NULL));

	/* Prepare shaders */
	programIndex = shaderHelper.loadShaderProgram("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");
	glUseProgram(programIndex); // load the shaders
	glClearColor(0.092f, 0.284f, 0.729f, 1.0f); // a deep blue
	shaderHelper.setShaderColor(programIndex, "color", 1.0f, 1.0f, 1.0f);

	/* Load models */
	mesh = new Mesh(programIndex);
	mesh->LoadMesh("Cthulhu");
}

/** Main OpenGL update method */
void Update() { 
	//cout << "Updating...\n";

	// update timing variables
	currentTime = (float)glfwGetTime();
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;
}
/** Main OpenGL draw call */
void Draw() {
	//cout << "Drawing...\n";

	// clear and redraw back
	glClear(GL_COLOR_BUFFER_BIT);

	// draw triangle
	shaderHelper.setShaderColor(programIndex, "color", 0.0f, 0.75f, 0.75f);
	//triangle->Draw(vec3(0.0f), vec3(0.25f));
	//for (int i = 0; i < NUM_OBJECTS; i ++) {
	//	gameObjects[i].Update(deltaTime);
	//}
	//triangle->Draw(vec3(0.0f), vec3(1.0f), vec3(0.0f, 1.0f, 0.0f), currentTime);
	mesh->Render(vec3(0.0f), vec3(0.2f), vec3(0.0f, 1.0f, 0.0f), currentTime);

	// flush buffer
	glFlush();
}

/** Mouse click callback */
void MouseButton (GLFWwindow* window, int button, int action, int mods)
{
	// currently unused
}
/** Mouse move callback */
void MouseMove (GLFWwindow* window, double xPixel, double yPixel)
{
	// get window width and height
	int winWidth, winHeight;
	glfwGetWindowSize(window, &winWidth, &winHeight);

	// set mouse position vector, scaled to window coordinates
	mousePos.x = (float)(xPixel/(winWidth/2) - 1.0f);
	mousePos.y = (float)(yPixel/(winHeight/-2) + 1.0f);
}

/** Main initialisation, setup OpenGL and open a rendering context */
int main() {
	/* Attempt to initialise GLFW */
	if (!glfwInit()) {
		cout << "Failure initilaising GLFW.\n";
		return -1;
	}

	windowPtr = glfwCreateWindow(750, 750, "OpenGL Review", NULL, NULL);
	// check success at creating context
	if (!windowPtr) {
		cout << "Failure initialising window context.\n";
		glfwTerminate();
		return -1;
	}

	/* Make the new context current */
	glfwMakeContextCurrent(windowPtr);

	/* Set callback functions */
	glfwSetMouseButtonCallback(windowPtr, MouseButton);
	glfwSetCursorPosCallback(windowPtr, MouseMove);
	
	glewExperimental = GL_TRUE;
	/* Attempt to initialise GLEW */
	if (glewInit() != GLEW_OK) {
		cout << "Failure initilaising GLEW.\n";
		return -1;
	}

	/* Initialises shaders and shapes */
	init();

	/* Loop until the user closes the window */
    while (!glfwWindowShouldClose(windowPtr))
    {
        /* Render here */
		Update();
		Draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(windowPtr);

        /* Poll for and process events */
        glfwPollEvents();
    }

	glfwTerminate();
	return 0;
}