#include "Camera.h"

/* Public section */
Camera::Camera()
{

}
Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	front = glm::vec3(0.0f, 0.0f, -1.0f);
	update();
}
Camera::~Camera()
{

}
void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;
	if (keys[GLFW_KEY_W]) // W pressed
	{
		position += front * velocity;
	}
	if (keys[GLFW_KEY_S]) // S pressed
	{
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_A]) // A pressed
	{
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_D]) // W pressed
	{
		position += right * velocity;
	}
}
void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw	  += xChange;
	pitch += yChange;

	// limit vertical angles
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	update();
}
glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}
glm::vec3 Camera::getCameraPosition()
{
	return position;
}
glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

/* Private section */
void Camera::update()
{
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	
	front = glm::normalize(front); // we are setting magnitude of 1 because we don't need how far front from us
	right = glm::normalize(glm::cross(front, worldUp)); // setting angle of front to worldUp
	up	  = glm::normalize(glm::cross(right, front));	// setting angle of right to front
}
