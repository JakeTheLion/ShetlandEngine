#include "Cthulhu.h"
#include <iostream>

/// Set Cthulhu default values
Cthulhu::Cthulhu()
{
	// Uses Cthulhu mesh
	mesh = GameManager::meshes["Cthulhu"];

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
			velocity = vec3(0.0f, 6.0f, 0.0f);
			break;
		// Spins around the x-axis (comes at you fast and furious)
		case 1:
			// Randomly move left or right
			if (rand() % 2 == 1)
				rotationAxis = vec3(3.0f, 0.0f, (rand() % 20)/10.0f - 1.0f);
			else
				rotationAxis = vec3(-3.0f, 0.0f, (rand() % 20) / 10.0f - 1.0f);

			rotationVelocity = 6.28f;
			velocity = vec3(0.0f, 0.0f, -3.0f);
			break;
		// Spins around the z-axis (cartwheels 4 dayz)
		case 2:
			rotationAxis = vec3(0.0f, 0.0f, (rand() % 10)/10.0f + 1.0f);
			rotationVelocity = 6.28f;
			velocity = vec3(1.0f, 0.0f, (float)(rand() % 3 - 1));
			break;
	}
}

// Cthulhu's update, does different stuff depending on AItype
void Cthulhu::Update(float dt)
{
	// Do some special checks on ones that spin like helicopters
	if (AItype == 0)
	{
		// Once we get low enough or high enough, bounce
		if ((position.y < boundingBox.e.y && velocity.y < 0.0f) || (position.y > 10.0f && velocity.y > 0.0f)) {
			velocity.y *= -1;
		}

		rotationVelocity = abs(velocity.y);
	}

	// Call the parent's update now that Cthulhu has had a chance to update
	GameObject::Update(dt);
}

Cthulhu::~Cthulhu()
{
}
