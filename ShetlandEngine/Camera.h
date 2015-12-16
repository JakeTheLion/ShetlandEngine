#pragma once
#include "GameObject.h"
#include <glm/gtx/transform.hpp>

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

		// Mutators
		void SetMinMaxYaw(float _minYaw, float _maxYaw);
		void SetMinMaxPitch(float _minPitch, float _maxPitch);
		void SetMoveSpeedScalars(float _moveForwardSpd, float _moveBackSpd, float _moveLeftSpd, float _moveRightSpd, float _moveUpSpd, float _moveDownSpd);

		// Public member functions
		void Update(float deltaTime, GLFWwindow* window);

		// Destructor
		~Camera();

	private:
		float yaw;					// horizontal rotation
		float pitch;				// vertical rotation
		float minYaw, maxYaw;		// min/max horizontal rotation
		float minPitch, maxPitch;	// min/max vertical rotation
		float moveLeftSpd, moveRightSpd, moveUpSpd, moveDownSpd, moveForwardSpd, moveBackSpd;
};

