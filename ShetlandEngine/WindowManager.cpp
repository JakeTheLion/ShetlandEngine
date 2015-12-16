#define _USE_MATH_DEFINES
#include "WindowManager.h"
#include <iostream>

#pragma region Define static variables
ShaderHelpers WindowManager::shaderHelper;
GLFWwindow* WindowManager::windowPtr;
int WindowManager::winWidth;
int WindowManager::winHeight;
vec2 WindowManager::mousePos;
GLuint WindowManager::programIndex;
GLuint WindowManager::cameraMatIndex;
mat4 WindowManager::projection;
Camera WindowManager::mainCamera;
#pragma endregion

/// Default constructor, calls init with default values (750x750 window named 'My Game')
WindowManager::WindowManager()
{
	// Initialize screen control variables
	mousePos = vec2(0.0f);		// mouse position in screen coords
	projection = mat4();		// camera projection matrix
	mainCamera = Camera();		// main game camera

	// Initialize GLFW
	GLFWinit(750, 750, "My Game");

	// Initialize GLEW
	GLEWinit();

	// Initialize window manager shaders and settings
	Init();
}

/// Parameterized constructor, just calls init with the passed values
WindowManager::WindowManager(int _winWidth, int _winHeight, string name)
{
	// Initialize screen control variables
	mousePos = vec2(0.0f);		// mouse position in screen coords
	projection = mat4();		// camera projection matrix
	mainCamera = Camera();		// main game camera

	// Initialize GLFW
	GLFWinit(_winWidth, _winHeight, name);

	// Initialize GLEW
	GLEWinit();

	// Initialize window manager shaders and settings
	Init();
}

/// Prepares GLFW and the game window
// @_winWidth	width of the window to create
// @_winHeight	height of the window to create
// @name		name of the window, will appear on the title bar
int WindowManager::GLFWinit(int _winWidth, int _winHeight, string name)
{
	// Attempt to initialise GLFW
	if (!glfwInit()) {
		cout << "Failure initilaising GLFW.\n";
		return -1;
	}

	// Create the window now the GLFW is ready
	windowPtr = glfwCreateWindow(_winWidth, _winHeight, name.c_str(), NULL, NULL);

	// Check success at creating the window context
	if (!windowPtr) {
		cout << "Failure initialising window context.\n";
		glfwTerminate();
		return -1;
	}

	// Store window width and height in reference variables
	glfwGetWindowSize(windowPtr, &winWidth, &winHeight);

	// Prepare window for use with camera controls
	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Make the new context the current one
	glfwMakeContextCurrent(windowPtr);

	// Set mouse callback functions
	glfwSetMouseButtonCallback(windowPtr, MouseButton);
	glfwSetCursorPosCallback(windowPtr, MouseMove);

	return 0;
}

/// Prepares GLEW
int WindowManager::GLEWinit()
{
	glewExperimental = GL_TRUE;

	// Attempt to initialise GLEW
	if (glewInit() != GLEW_OK) {
		cout << "Failure initilaising GLEW.\n";
		return -1;
	}

	return 0;
}

/// Prepares the window manager's shaders
void WindowManager::Init()
{
	glEnable(GL_DEPTH_BUFFER);
	glEnable(GL_DEPTH_TEST);

	//== Prepare shaders
	programIndex = shaderHelper.loadShaderProgram("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");
	glUseProgram(programIndex);
	glClearColor(0.092f, 0.284f, 0.729f, 1.0f);		// this default clear color is a deep sky blue

	//= Prepare camera variables
	// Get camera matrix address in shader
	cameraMatIndex = glGetUniformLocation(programIndex, "cameraMatrix");
	// Create projection matrix
	projection = perspective((float)M_PI_2, (float)(winWidth / winHeight), 0.01f, 1000.0f);
}

/// Update WindowManager variables, namely the game camera
int WindowManager::Update(float deltaTime)
{
	// Update camera, moves based on mouse movement and delta time
	mainCamera.Update(deltaTime, windowPtr);

	// Get the camera's position
	vec3 cameraPos = mainCamera.GetPosition();

	// Create a view matrix based on the camera's orientation
	mat4 view = lookAt(mainCamera.GetPosition(), mainCamera.GetLookAt(), mainCamera.GetUp());

	// Push a final view/projection matrix to the shader
	glProgramUniformMatrix4fv(programIndex, cameraMatIndex, 1, false, &(projection * view)[0][0]);

	return 0;
}

/// Returns the index of the shader program
GLuint WindowManager::GetProgram()
{
	return programIndex;
}

/// Returns the window manager's pointer to the main GLFW window
GLFWwindow* WindowManager::GetWindow()
{
	return windowPtr;
}

/// GLFW mouse click callback
void WindowManager::MouseButton(GLFWwindow* window, int button, int action, int mods)
{
	// currently unused
}

/// GLFW mouse move callback
void WindowManager::MouseMove(GLFWwindow* window, double xPixel, double yPixel)
{
	// Get window width and height
	glfwGetWindowSize(window, &winWidth, &winHeight);

	// Put mouse screen pos into temporary vector
	vec2 tempMousePos = vec2(0.0f);
	tempMousePos.x = (float)(xPixel / (winWidth / 2) - 1.0f);
	tempMousePos.y = (float)(yPixel / (winHeight / -2) + 1.0f);

	// Turn camera based on difference between last mouse pos and this one
	mainCamera.turn(tempMousePos.x - mousePos.x, tempMousePos.y - mousePos.y);

	// Set mouse position vector, scaled to window coordinates
	mousePos = tempMousePos;
}

WindowManager::~WindowManager()
{
}
