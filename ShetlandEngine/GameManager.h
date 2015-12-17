#pragma once
#include <map>
#include "Camera.h"
#include "ShaderHelpers.h"
#include "ColliderTree.h"
#include "WindowManager.h"
#include "Mesh.h"
#include "Cthulhu.h"

class GameManager
{
	public:
		// Constructor
		GameManager(int spawns);

		// A global map of meshes to file names
		static map<string, Mesh*> meshes;
		// Loads a mesh into the meshes map
		static int LoadMesh(string fileName);

		// Public global word variables
		static vec3 gravity;

		//== Public member functions
		// Spawns a completely in-method defined basic game object in the world
		static void SpawnObject (string meshFile,							// pointer to the object's mesh
						  vec3 position = vec3(),							// game object's starting position
						  vec3 velocity = vec3(),							// game object's starting velocity
						  vec3 scale = vec3(1.0f),							// scale along each axis (x/y/z scale = x/y/z/ of vec3)
						  vec3 rotationAxis = vec3(0.0f, 1.0f, 0.0f),		// axis the game object will rotate around
						  float rotation = 0.0f,							// object's starting rotation in radians around its rotation axis
						  float rotationVelocity = 0.0f,					// rotation in radians per tick around its rotation axis
						  float mass = 1.0f);								// mass, affects strength of force impulses

		// Spawns a pre-defined game object
		static void SpawnObject(GameObject* object);						// spawn a prepared game object
		static void SpawnObject(GameObject* object, vec3 pos);				// spawn a game object and force its position
		static void Delete(GameObject* object);								// attempts to delete the specified object

		// Main update/render
		static int Loop();
		static void Update(float dt);
		static void Render();

		// Throw rock
		static void throwRock();

		// Destructor
		~GameManager();

	private:
		// Game variables
		static vector<GameObject*> worldObjects;// Contains all objects in the game world
		static ColliderTree worldOctree;		// The entire world octree
		static float currentTime;				// Time elapsed between now and the program start
		static float prevTime;					// Time elapsed between the last frame and the program start
		static float deltaTime;					// Time between this frame and the last
		static bool isHeld;						// If the rock is currently held
		static GameObject rock;					// Rock to throw at Cthulhu
		static int score;						// Number of Cthulhus killed
		static int cthulhus;					// Number of total Cthulhus

		// Unused
		GameManager();
};