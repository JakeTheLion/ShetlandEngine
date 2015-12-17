#include "GameManager.h"
#include "WindowManager.h"
#include <iostream>
#include <string>

#pragma region Define static variables
vec3 GameManager::gravity;
vector<GameObject*> GameManager::worldObjects;
ColliderTree GameManager::worldOctree;
map<string, Mesh*> GameManager::meshes;
float GameManager::currentTime;
float GameManager::prevTime;
float GameManager::deltaTime;
bool GameManager::isHeld;
GameObject GameManager::rock;
int GameManager::score;
int GameManager::cthulhus;
#pragma endregion

/// GameWorld Ctor - Creates what's needed to run the world
GameManager::GameManager(int spawns)
{
	// Set spawn count
	cthulhus = spawns;

	// Prepare the game's window manager
	WindowManager windowManager = WindowManager(750, 750, "Shetland Engine");

	// Initialize the array of game objects
	worldObjects = vector<GameObject*>();

	// Initialize the world octree
	worldOctree = ColliderTree(vec3(), vec3(50.0f, 50.0f, 50.0f));

	// Prepare world variables
	gravity = vec3(0.0f, -30.0f, 0.0f);

	// Initialize the meshes map
	meshes = map<string, Mesh*>();

	// Place rock in hand
	isHeld = true;

	// Create rock
	rock = GameObject("Rock", vec3(), vec3(), vec3(1.0f), vec3(0.0f, 1.0f, 0.0f), 0.0f, 2.0f);

	// Set score
	score = 0;
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
	GameObject* newObj = new GameObject(meshFile, position, velocity, scale, rotationAxis, rotation, rotationVelocity, mass);

	// Add it to the world vector of objects
	worldObjects.push_back(newObj);
}

/// Spawns a pre-defined game object and adds it to the world
// @position		The object's starting position (defaults to origin)
void GameManager::SpawnObject(GameObject* object, vec3 pos)
{
	// Update object with passed info
	object->SetPosition(pos);
	/*
	object.SetVelocity(velocity);
	object.SetScale(scale);
	object.SetRotationAxis(rotationAxis);
	object.SetRotation(rotation);
	object.SetRotationVelocity(rotationVelocity);
	object.SetMass(mass);
	*/

	// Add it to the world vector of objects
	worldObjects.push_back(object);
}

/// Attempts to delete an object from the list of world objects
// @object	The game object to delete
void GameManager::Delete(GameObject * object)
{
	// Find the object's position in the world object list
	int objPos = find(worldObjects.begin(), worldObjects.end(), object) - worldObjects.begin();
	
	// Found the object!
	if (objPos < worldObjects.size())
	{
		worldObjects.erase(worldObjects.begin() + objPos);
	}
}

/// Spawns a pre-defined game object and adds it to the world
void GameManager::SpawnObject(GameObject* object)
{
	// Add it to the world vector of objects
	worldObjects.push_back(object);
}

/// Loads a mesh into the meshes map
int GameManager::LoadMesh(string fileName)
{
	// Attempt to find the mesh in case it's already stored so we don't load it twice
	auto meshSearcher = meshes.find(fileName);

	// Call load mesh if it wasn't found in the mesh map
	if (meshSearcher == meshes.end()) {
		meshes[fileName] = new Mesh(WindowManager::GetProgram(), fileName);
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

	// CLEAR OCTREE
	worldOctree = ColliderTree(vec3(), vec3(50.0f, 50.0f, 50.0f));

	// Loop through objects
	for (size_t i = 0; i < worldObjects.size(); ++i)
	{
		if (worldObjects[i]->isDead)
		{
			worldObjects.erase(worldObjects.begin() + i);
			i--;
		}
		worldOctree.add(worldObjects[i]);
		worldObjects[i]->Update(dt);
	}
	rock.Update(dt);

	// Place rock in hand if held
	if(isHeld)
	{
		rock.SetPosition(WindowManager::GetCamera().GetLookAt() - WindowManager::GetCamera().GetUp() / 2.0f );
	}
	// Else check if it hit something
	else
	{
		GameObject* hit = worldOctree.collidesWith(&rock);
		if (hit != nullptr)
		{
			// Check if that was a Cthulhu and delete
			if (hit->name == "Cthulhu")
			{
				hit->isDead = true;
				score++;
				cout << "Cthulhu-man slain!!!\n";
				cout << "Score: " << score << endl;

				// Give game end message
				if (score == cthulhus)
				{
					cout << "You've killed all the Cthulhu-men, and ended \nthe Cult of Cthulhu's plans for world domination! \nCool beans, yo!\n";
					cout << "Press 'Q' to quit.\n";
				}

				// reset rock to hand
				rock.SetVelocity(vec3());
				rock.gravity = false;
				isHeld = true;
			}

			// reset rock to hand
			rock.SetVelocity(vec3());
			rock.gravity = false;
			isHeld = true;
		}
		if (rock.GetPosition().y < 0.0f || rock.GetPosition().z > 10.0f)
		{
			// reset rock to hand
			rock.SetVelocity(vec3());
			rock.gravity = false;
			isHeld = true;
		}
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
		worldObjects[i]->Render();
	}
	rock.Render();

	// Flush the render buffer
	glFlush();
}

/// Throws rock forward
void GameManager::throwRock()
{
	if (!isHeld)
		return;
	rock.SetVelocity(WindowManager::GetCamera().GetForward() * 30.0f);
	rock.gravity = true;
	isHeld = false;
}

GameManager::~GameManager()
{
}
