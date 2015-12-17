#include "Cthulhu.h"
#include <iostream>

/// Set Cthulhu default values
Cthulhu::Cthulhu()
{
	// Uses Cthulhu mesh
	mesh = GameManager::meshes["Cthulhu"];
	name = "Cthulhu";

	// Randomized scales
	scale = vec3(0.28f);

	// Randomize the AI type
	AItype = rand() % 3;
	rotation = (rand() % 628) / 100;

	// Update initial variables depending on AI type
	switch (AItype)
	{
		// Spins around normally (around the y-axis) (flies like a helicopter)
		case 0:
			velocity = vec3(0.0f, (rand() % 9) / 3.0f + 4, 0.0f);
			break;
		// Spins around the x-axis (comes at you fast and furious)
		case 1:
			rotationAxis = vec3(1.0f, 0.0f, 0.0f);
			// Randomly move forward or back
			if (rand() % 2 == 1) 
			{
				velocity = vec3(0.0f, 0.0f, (rand() % 8) / -4.0f - 3);
				rotationVelocity = velocity.z - 4;
			}
			else
			{
				velocity = vec3(0.0f, 0.0f, (rand() % 8) / 4.0f + 3);
				rotationVelocity = velocity.z + 4;
			}
			break;
		// Spins around the z-axis (cartwheels 4 dayz)
		case 2:
			rotationAxis = vec3(0.0f, 0.0f, 1.0f);
			if (rand() % 2 == 1)
			{
				velocity = vec3((rand() % 8) / -4.0f - 3, 0.0f, 0.0f);
				rotationVelocity = -1 * velocity.x + 4;
			}
			else
			{
				velocity = vec3((rand() % 8) / 4.0f + 3, 0.0f, 0.0f);
				rotationVelocity = -1 * velocity.x - 4;
			}
			break;
	}
}

// Cthulhu's update, does different stuff depending on AItype
void Cthulhu::Update(float dt)
{
	// Update movement to bounce around gallery
	switch (AItype)
	{
		// Helicopters
		case 0:
			// Once we get low enough or high enough, bounce
			if ((position.y <= boundingBox.e.y && velocity.y < 0.0f) || (position.y >= 10.0f && velocity.y > 0.0f)) {
				velocity.y *= -1;
			}

			rotationVelocity = pow(abs(position.y), 1.5f);
			break;
		// Attackers
		case 1:
			// Bounce at back wall or in front of player
			if ((position.z <= -8.0f && velocity.z < 0.0f) || (position.z >= 10.0f && velocity.z > 0.0f)) {
				velocity.z *= -1;
				rotationVelocity *= -1;
			}
			break;
		// Evaders
		case 2:
			// Bounce against left and right walls
			if ((position.x <= -9.0f && velocity.x < 0.0f) || (position.x >= 9.0f && velocity.x > 0.0f)) {
				velocity.x *= -1;
				rotationVelocity *= -1;
			}
			break;
	}
	// Call the parent's update now that Cthulhu has had a chance to update
	GameObject::Update(dt);
}

Cthulhu::~Cthulhu()
{
}
