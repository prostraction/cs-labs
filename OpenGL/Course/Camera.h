#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera();

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::mat4 calculateViewMatrix();
	glm::vec3 getCameraPosition();	
	glm::vec3 getCameraDirection();

private:
	glm::vec3 position; // position of the camera [x,y,z]
	glm::vec3 front;	// direction in front position 
	glm::vec3 up;		// direction of the up position of the camera
	glm::vec3 right;	// define x-axis, vec facing right of the camera 
	glm::vec3 worldUp;  // direction of the world (sky)
	
	GLfloat yaw;		// turning angle: looking left and right
	GLfloat pitch;		// turning angle: looking up and down

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void update();

};

