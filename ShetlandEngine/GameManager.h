#pragma once
#include <map>
#include "Camera.h"
#include "ShaderHelpers.h"
#include "ColliderTree.h"
#include "WindowManager.h"
#include "Mesh.h"

class GameManager
{
	public:
		// Constructor
		GameManager();

		// A global map of meshes to file names
		static map<string, Mesh*> meshes;
		// Loads a mesh into the meshes map
		static int LoadMesh(string fileName);

		// Public global word variables
		static vec3 gravity;

		//== Public member functions
		// Spawns a game object in the world
		static void SpawnObject (string meshFile,							// pointer to the object's mesh
						  vec3 position = vec3(),							// game object's starting position
						  vec3 velocity = vec3(),							// game object's starting velocity
						  vec3 scale = vec3(1.0f),							// scale along each axis (x/y/z scale = x/y/z/ of vec3)
						  vec3 rotationAxis = vec3(0.0f, 1.0f, 0.0f),		// axis the game object will rotate around
						  float rotation = 0.0f,							// object's starting rotation in radians around its rotation axis
						  float rotationVelocity = 0.0f,					// rotation in radians per tick around its rotation axis
						  float mass = 1.0f);								// mass, affects strength of force impulses

		// Main update/render
		static int Loop();
		static void Update(float dt);
		static void Render();

		// Destructor
		~GameManager();

	private:
		// Game variables
		static vector<GameObject> worldObjects;	// Contains all objects in the game world
		static ColliderTree worldOctree;		// The entire world octree
		static float currentTime;				// Time elapsed between now and the program start
		static float prevTime;					// Time elapsed between the last frame and the program start
		static float deltaTime;					// Time between this frame and the last
};