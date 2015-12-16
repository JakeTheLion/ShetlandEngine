#include "GameManager.h"
#include "WindowManager.h"
#include <iostream>

#pragma region Define static variables
vec3 GameManager::gravity;
vector<GameObject> GameManager::worldObjects;
ColliderTree GameManager::worldOctree;
map<string, Mesh*> GameManager::meshes;
float GameManager::currentTime;
float GameManager::prevTime;
float GameManager::deltaTime;
#pragma endregion

/// GameWorld Ctor - no params, automatically creates what's needed to run the world
GameManager::GameManager()
{
	// Prepare the game's window manager
	WindowManager windowManager = WindowManager(750, 750, "Shetland Engine");

	// Initialize the array of game objects
	worldObjects = vector<GameObject>();

	// Initialize the world octree
	worldOctree = ColliderTree(vec3(), vec3(50.0f, 50.0f, 50.0f));

	// Prepare world variables
	gravity = vec3(0.0f, 0.98f, 0.0f);

	// Initialize the meshes map
	meshes = map<string, Mesh*>();
}

/// Spawns a game object and adds it to the world objects list with all of the specified parameters
// @meshPtr				The mesh that the game object will render
// @position			The object's starting position (defaults to origin)
// @velocity			The game object's initial velocity (defaults to 0 in all axes)
// @rotationAxis		The axis the object rotates around (defaults to y axis)
// @rotation			How far the object is rotated around its axis in radians (defaults to 0)
// @rotationVelocity	How many radians per tick the object should rotate (defaults to 0)
// @scale				The object's scale along each axis (defaults to 1)
// @mass				The object's mass, which affects the strength of applied forces (defaults to 1)
void GameManager::SpawnObject(string meshFile, vec3 position, vec3 velocity, vec3 scale, vec3 rotationAxis, float rotation, float rotationVelocity, float mass)
{
	// Instantiate the new object
	GameObject newObj = GameObject(meshFile, position, velocity, scale, rotationAxis, rotation, rotationVelocity, mass);

	// Add it to the world vector of objects
	worldObjects.push_back(newObj);
}

/// Loads a mesh into the meshes map
int GameManager::LoadMesh(string fileName)
{
	// Attempt to find the mesh in case it's already stored so we don't load it twice
	auto meshSearcher = meshes.find(fileName);

	// Call load mesh if it wasn't found in the mesh map
	if (meshSearcher == meshes.end()) {
		meshes[fileName] = new Mesh(WindowManager::GetProgram(), fileName);
		cout << "Loading mesh " << fileName.c_str() << "\n";
	}
	
	return -1;
}

/// The main window loop, also passed down to WindowManager
int GameManager::Loop()
{
	// Loop until the user closes the window 
	while (!glfwWindowShouldClose(WindowManager::GetWindow()))
	{
		// Call main GameManager update to update game positions and timing variables
		Update(deltaTime);

		// Handle WindowManager's update now that deltaTime is updated
		WindowManager::Update(deltaTime);

		// Last, render all objects
		// Doing this last and separate from update so that it's after WindowManager's update
		// This is better because the camera's movement this frame will have happened by now
		// Good future proof for out of frame objects not being rendered
		Render();

		// Swap front and back buffers
		glfwSwapBuffers(WindowManager::GetWindow());

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

/// Updates all game objects
void GameManager::Update(float dt)
{
	// Update timing variables
	currentTime = (float)glfwGetTime();
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;

	// Loop through objects
	for (size_t i = 0; i < worldObjects.size(); ++i)
	{
		worldObjects[i].Update(dt);
	}
}

/// Renders all game objects
void GameManager::Render()
{
	// Clear and redraw back
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Loop through objects
	for (size_t i = 0; i < worldObjects.size(); ++i)
	{
		worldObjects[i].Render();
	}

	// Flush the render buffer
	glFlush();
}

GameManager::~GameManager()
{
}
