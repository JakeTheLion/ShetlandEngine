#define _USE_MATH_DEFINES
#include "WindowManager.h"
#include "GameManager.h"
#include "Cthulhu.h"
#include <time.h>
#include <iostream>

using namespace std;

// Program entry point
int main() {
	// Set the random
	srand((unsigned int)time(NULL));

	// Determine number of spawned Cthulhus
	int spawns = 25;

	// Have to create window manager first to create rendering context
	GameManager gameManager = GameManager(spawns);

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

	// Spawn cthulhus
	for (int i = 0; i < spawns; ++i)
		GameManager::SpawnObject(new Cthulhu(), vec3((rand() % 180)/10.0f - 9.0f, 1.0f, (rand() % 180) / 10.0f - 7.0f));

	// MOVE CAMERA
	WindowManager::GetCamera().SetPosition(vec3(0.0f, 1.0f, -9.5f));
	WindowManager::GetCamera().SetMinMaxYaw(M_PI, M_PI);
	WindowManager::GetCamera().SetMinMaxPitch(-M_PI_4, M_PI_2);
	WindowManager::GetCamera().SetMoveSpeedScalars(0.0f, 0.0f, 4.0f, 4.0f, 0.0f, 0.0f);

	// Intro
	cout << "=======================================\n";
	cout << "!!                                   !!\n";
	cout << "!!  Welcome to The Ball of Cthulhu!  !!\n";
	cout << "!!                                   !!\n";
	cout << "=======================================\n\n";
	cout << "Throw The Ball at the Cthulhu-men to destroy \nthem and earn points!\n\n";
	cout << "A/D:     Move left/right\n";
	cout << "Mouse:   Aim rock up/down\n";
	cout << "LMB:     Throw The Ball\n\n";

	// START MAIN GAME LOOP
	GameManager::Loop();

	return 0;
}