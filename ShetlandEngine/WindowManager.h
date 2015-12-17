#pragma once
#include "ShaderHelpers.h"
#include "Camera.h"
#include "GameManager.h"

class WindowManager
{
	public:
		// Constructors
		WindowManager();
		WindowManager(int _winWidth, int _winHeight, string name = "My Game");

		// Accessors
		static GLuint GetProgram();			// returns the program's index
		static GLFWwindow* GetWindow();		// returns the window pointer
		static Camera& GetCamera();			// returns the main game camera

		// The WindowManager's update, which handles camera updates, etc
		static int Update(float deltaTime);
		
		// Destructors
		~WindowManager();

	private:
		//== Private member functions
		// Initializes GLFW and creates a window context
		int GLFWinit(int _winWidth,			// the width of the game window
					 int _winHeight,		// the height of the game window
					 string name);			// the name of the game window

		// Initializes GLEW
		int GLEWinit();

		// Prepares GL variables and shaders
		void Init();

		// Mouse function callbacks for GLFW
		static void MouseButton(GLFWwindow* window, int button, int action, int mods);
		static void MouseMove(GLFWwindow* window, double xPixel, double yPixel);

		//== Private member data
		static ShaderHelpers shaderHelper;	// creates and loads shader and shader programs
		static GLFWwindow* windowPtr;		// a GLFW window context
		static int winWidth, winHeight;		// stores reference to window width and height
		static vec2 mousePos;				// stores position of mouse for camera controls
		static GLuint programIndex;			// index of shader program in vertex shader
		static GLuint cameraMatIndex;		// index of camera matrix in vertex shader
		static mat4 projection;				// projection matrix for camera controls
		static Camera mainCamera;			// main game camera
};

