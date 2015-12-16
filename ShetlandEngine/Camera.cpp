#define _USE_MATH_DEFINES

#include "Camera.h"
#include <math.h>


/* DEFAULT CONSTRUCTOR */
Camera::Camera()
{
	// Default starting position and rotation values
	pitch = 0.0f;
	yaw = M_PI;
	position = vec3(0.0f, 1.0f, 0.0f);

	// Default min/max rotation values to clamp yaw/pitch
	minYaw = -(float)M_PI;
	maxYaw = (float)M_PI;
	minPitch = -(float)M_PI_2;
	maxPitch = (float)M_PI_2;

	// Default movement speed values 
	float moveLeftSpd = moveRightSpd = moveUpSpd = moveDownSpd = moveForwardSpd = moveBackSpd = 3.0f;
}

/// Turns the camera by dx and dy
// @dx		Amount to add to the yaw, clamped to the camera's minYaw/maxYaw
// @dy		Amount to add to the pitch, clamped to the camera's minPitch/maxPitch
void Camera::turn(float dx, float dy)
{
	// Add to yaw/pitch
	yaw += dx;
	pitch += dy;

	// Lock yaw within min and max
	if (yaw < minYaw || yaw > maxYaw) {
		yaw = clamp(yaw, minYaw, maxYaw);
	}

	// Do the same for pitch - lock within min and max
	if (pitch < minPitch || pitch > maxPitch) {
		pitch = clamp(pitch, minPitch, maxPitch);
	}
}

/** ACCESSORS **/
// Returns forward vector
vec3 Camera::GetForward()
{
	vec3 forward;

	forward.x = cos(pitch)*sin(yaw);  // calculate x
	forward.y = sin(pitch);			  // calculate y
	forward.z = -cos(pitch)*cos(yaw); // calculate z

	return forward;
}

/// Returns location camera is looking at
vec3 Camera::GetLookAt()
{
	return GetPosition() + GetForward();
}

/// Return up vector from camera
vec3 Camera::GetUp()
{
	vec3 up;

	up.x = cos(pitch + (float)M_PI/2)*sin(yaw);	// calculate x
	up.y = sin(pitch + (float)M_PI/2);			// calculate y
	up.z = -cos(pitch + (float)M_PI/2)*cos(yaw);	// calculate z

	return up;
}

/// Return right vector from camera
vec3 Camera::GetRight()
{
	// Right will be cross of up and forward
	// Becaues it's perpendicular to both
	return cross(GetForward(), GetUp());
}

/// Sets the min and max allowed yaw (horizontal rotation) for the camera
// @_minYaw		The new minimum camera yaw allowed, in radians
// @_maxYaw		The new maximum camera yaw allowed, in radians
void Camera::SetMinMaxYaw(float _minYaw, float _maxYaw)
{
	minYaw = _minYaw;
	maxYaw = _maxYaw;
}

/// Sets the min and max allowed pitch (vertical rotation) for the camera
// @_minYaw		The new minimum camera pitch allowed, in radians
// @_maxYaw		The new maximum camera pitch allowed, in radians
void Camera::SetMinMaxPitch(float _minPitch, float _maxPitch)
{
	minPitch = _minPitch;
	maxPitch = _maxPitch;
}

void Camera::SetMoveSpeedScalars(float _moveForwardSpd, float _moveBackSpd, float _moveLeftSpd, float _moveRightSpd, float _moveUpSpd, float _moveDownSpd)
{
	moveForwardSpd = _moveForwardSpd;
	moveBackSpd = _moveBackSpd;
	moveLeftSpd = _moveLeftSpd;
	moveRightSpd = _moveRightSpd;
	moveUpSpd = _moveUpSpd;
	moveDownSpd = _moveDownSpd;
}

void Camera::Update(float deltaTime, GLFWwindow* window)
{
	// detect wasd key presses and move camera
	vec3 cameraMove = vec3();

	// move camera based on inputs and its forward/right/up vectors
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraMove += GetForward() * moveForwardSpd;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraMove -= GetRight() * moveLeftSpd;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraMove -= GetForward() * moveBackSpd;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraMove += GetRight() * moveRightSpd;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraMove += GetUp() * moveUpSpd;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraMove -= GetUp() * moveDownSpd;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwTerminate();
		std::exit(0);
	}

	// move the camera from the movement and delta time
	vec3 cameraPos = GetPosition();
	SetPosition(cameraPos + cameraMove*deltaTime);
}

Camera::~Camera()
{
}
