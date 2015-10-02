#include "GameObject.h"
#include <glm/gtx/transform.hpp>
#pragma once

class Camera: public GameObject
{
	public:
		// Constructor
		Camera();
		void turn(float dx, float dy); // turns the camera by an x and y rotation

		// Accessors
		vec3 GetForward();
		vec3 GetLookAt();
		vec3 GetUp();
		vec3 GetRight();
		void Update(float deltaTime, GLFWwindow* window);

		// Destructor
		~Camera();

	private:
		float yaw;			// horizontal rotation
		float pitch;		// vertical rotation
};

