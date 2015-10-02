#define _USE_MATH_DEFINES

#include "Camera.h"
#include <math.h>

/* DEFAULT CONSTRUCTOR */
Camera::Camera()
{
	pitch = 0.0f;
	yaw = 0.0f;
	position = vec3(0.0f, 0.0f, 1.0f);
}

// Turns the camera about its axis
void Camera::turn(float dx, float dy)
{
	yaw += dx;
	pitch += dy;
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
// Returns location camera is looking at
vec3 Camera::GetLookAt()
{
	return GetPosition() + GetForward();
}
// Return up vector from camera
vec3 Camera::GetUp()
{
	vec3 up;

	up.x = cos(pitch + M_PI/2)*sin(yaw);	// calculate x
	up.y = sin(pitch + M_PI /2);			// calculate y
	up.z = -cos(pitch + M_PI /2)*cos(yaw);	// calculate z

	return up;
}
// Return right vector from camera
vec3 Camera::GetRight()
{
	// Right will be cross of up and forward
	// Becaues it's perpendicular to both
	return cross(GetForward(), GetUp());
}

void Camera::Update(float deltaTime, GLFWwindow* window)
{
	// detect wasd key presses and move camera
	vec3 cameraMove = vec3();

	// move camera based on inputs and its forward/right/up vectors
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraMove += GetForward();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraMove -= GetRight();
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraMove -= GetForward();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraMove += GetRight();
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraMove += GetUp();
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraMove -= GetUp();
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
