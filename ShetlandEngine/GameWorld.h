#pragma once
#include "Camera.h"
#include "ShaderHelpers.h"
#include "ColliderTree.h"

class GameWorld
{
	public:
		// Constructor
		GameWorld();

		// Public global word variables
		static vec3 GRAVITY;

		// Destructor
		~GameWorld();

	private:
		static vector<GameObject> worldObjects;	// Contains all objects in the game world
		static ColliderTree worldOctree;			// The entire world octree
};