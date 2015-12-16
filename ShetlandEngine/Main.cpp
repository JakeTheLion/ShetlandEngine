#define _USE_MATH_DEFINES
#include "WindowManager.h"
#include "GameManager.h"
#include <time.h>

// Program entry point
int main() {
	// Set the random
	srand((unsigned int)time(NULL));

	// Have to create window manager first to create rendering context
	GameManager gameManager = GameManager();

	// SPAWN OBJECTS
	GameManager::SpawnObject("Cthulhu", vec3(), vec3(), vec3(0.25f));
	GameManager::SpawnObject("Cthulhu", vec3(1.0f, 0.0f, 0.0f), vec3(), vec3(0.25f));
	GameManager::SpawnObject("Cthulhu", vec3(-1.0f, 0.0f, 0.0f), vec3(), vec3(0.25f));

	GameManager::Loop();
}