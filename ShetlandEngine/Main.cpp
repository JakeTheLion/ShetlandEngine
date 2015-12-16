#define _USE_MATH_DEFINES
#include "WindowManager.h"
#include "GameManager.h"
#include "Cthulhu.h"
#include <time.h>

// Program entry point
int main() {
	// Set the random
	srand((unsigned int)time(NULL));

	// Have to create window manager first to create rendering context
	GameManager gameManager = GameManager();

	// LOAD MESHES
	GameManager::LoadMesh("Cthulhu");
	GameManager::LoadMesh("Ground");	// Technically don't need this here, the ground will try to load it when I create it below. Nice to have it all in one place tho.

	// SPAWN OBJECTS
	// Ground
	GameObject ground = GameObject("Ground", vec3(), vec3(), vec3(20.0f, 1.0f, 20.0f));
	GameManager::SpawnObject(&ground);

	// Walls - use ground mesh, rotated
	GameObject leftWall = GameObject("Ground", vec3(-10.0f, 10.0f, 0.0f), vec3(), vec3(20.0f, 1.0f, 20.0f), vec3(0.0f, 0.0f, 1.0f), M_PI_2);
	GameManager::SpawnObject(&leftWall);
	GameObject rightWall = GameObject("Ground", vec3(10.0f, 10.0f, 0.0f), vec3(), vec3(20.0f, 1.0f, 20.0f), vec3(0.0f, 0.0f, 1.0f), M_PI_2);
	GameManager::SpawnObject(&rightWall);
	GameObject backWall = GameObject("Ground", vec3(0.0f, 10.0f, 10.0f), vec3(), vec3(20.0f, 1.0f, 20.0f), vec3(1.0f, 0.0f, 0.0f), M_PI_2);
	GameManager::SpawnObject(&backWall);
	GameObject frontWall = GameObject("Ground", vec3(0.0f, 10.0f, -10.0f), vec3(), vec3(20.0f, 1.0f, 20.0f), vec3(1.0f, 0.0f, 0.f), M_PI_2);
	GameManager::SpawnObject(&frontWall);

	// Spawn 50 cthulhus
	for (int i = 0; i < 50; ++i)
		GameManager::SpawnObject(new Cthulhu(), vec3((rand() % 180)/10.0f - 9.0f, 1.0f, (rand() % 180) / 10.0f - 9.0f));

	// MOVE CAMERA
	WindowManager::GetCamera().SetPosition(vec3(0.0f, 1.0f, -10.0f));
	WindowManager::GetCamera().SetMinMaxYaw(M_PI, M_PI);
	WindowManager::GetCamera().SetMinMaxPitch(-M_PI_4, M_PI_4);
	WindowManager::GetCamera().SetMoveSpeedScalars(0.0f, 0.0f, 4.0f, 4.0f, 0.0f, 0.0f);

	// START MAIN GAME LOOP
	GameManager::Loop();

	return 0;
}