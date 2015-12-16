#define _USE_MATH_DEFINES
#include <glew.h>
#include <glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include "ShaderHelpers.h"
#include "Camera.h"
#include <time.h>
#include <math.h>
using namespace std;
using namespace glm;

/** Global Variables */
ShaderHelpers shaderHelper = ShaderHelpers();
GLuint programIndex, cameraMatIndex;	// index of shader program and camera matrix in vertex shader
Mesh* mesh;								// basic empty mesh
mat4 projection = mat4();				// projection matrix for camera controls
Camera mainCamera = Camera();			// main game camera
vec2 mousePos = vec2(0.0f);				// stores position of mouse for camera controls
float currentTime, prevTime, deltaTime; // time control vars for object physics
int winWidth, winHeight;				// stores reference to window width and height
const int NUM_OBJECTS = 100;			// max number of game objects
GameObject gameObjects[NUM_OBJECTS];	// empty array to store game objects
/* Create an OpenGL window context */
GLFWwindow* windowPtr;

/** Main initialisation for shaders and shapes */
void init() {
	glEnable(GL_DEPTH_BUFFER);
	glEnable(GL_DEPTH_TEST);

	srand((unsigned int)time(NULL));

	/* Prepare shaders */
	programIndex = shaderHelper.loadShaderProgram("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");
	glUseProgram(programIndex);					// load the shaders
	glClearColor(0.092f, 0.284f, 0.729f, 1.0f); // a deep blue

	/* Prepare camera variables */
	cameraMatIndex = glGetUniformLocation(programIndex, "cameraMatrix"); // get address of the shader cameraMatrix variable
	projection = perspective((float)M_PI_2, (float)(winWidth / winHeight), 0.01f, 1000.0f);

	/* Load models */
	mesh = new Mesh(programIndex, "Cthulhu");
}

/** Main OpenGL update method */
void Update() {
	// update timing variables
	currentTime = (float)glfwGetTime();
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;

	// update camera (movement)
	mainCamera.Update(deltaTime, windowPtr);

	// move the camera from the movement and delta time
	vec3 cameraPos = mainCamera.GetPosition();

	// get view matrix
	mat4 view = lookAt(mainCamera.GetPosition(), mainCamera.GetLookAt(), mainCamera.GetUp());

	// push view matrix to the shader
	glProgramUniformMatrix4fv(programIndex, cameraMatIndex, 1, false, &(projection * view)[0][0]);
}

/** Main OpenGL draw call */
void Draw() {
	//cout << "Drawing...\n";

	// clear and redraw back
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw triangle
	shaderHelper.setShaderColor(programIndex, "color", 0.0f, 0.75f, 0.75f);
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
	glfwGetWindowSize(window, &winWidth, &winHeight);

	// put mouse screen pos into temporary vector
	vec2 tempMousePos = vec2(0.0f);
	tempMousePos.x = (float)(xPixel / (winWidth / 2) - 1.0f);
	tempMousePos.y = (float)(yPixel / (winHeight / -2) + 1.0f);

	// turn camera based on difference between last mouse pos and this one
	mainCamera.turn(tempMousePos.x - mousePos.x, tempMousePos.y - mousePos.y);

	// set mouse position vector, scaled to window coordinates
	mousePos = tempMousePos;
}

/** Main initialisation, setup OpenGL and open a rendering context */
int main() {

	/* Attempt to initialise GLFW */
	if (!glfwInit()) {
		cout << "Failure initilaising GLFW.\n";
		return -1;
	}

	windowPtr = glfwCreateWindow(750, 750, "Shetland Engine", NULL, NULL);
	// check success at creating context
	if (!windowPtr) {
		cout << "Failure initialising window context.\n";
		glfwTerminate();
		return -1;
	}

	/* Get window width and height */
	glfwGetWindowSize(windowPtr, &winWidth, &winHeight);

	/* Setup window for camera controls */
	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
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