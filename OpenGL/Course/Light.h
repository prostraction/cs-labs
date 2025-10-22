#pragma once

#include <glm\glm.hpp>
#include <GL\glew.h>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
	~Light();

	ShadowMap* GetShadowMap();

	//void UseLight(GLuint ambientIntensityLocation, GLuint ColorLocation, GLuint diffuseIntensityLocation);

protected:
	// entire light:
	glm::vec3 color;

	// ambient:
	GLfloat ambientIntensity;

	// diffuse:
	//glm::vec3 direction;
	GLfloat diffuseIntensity;

	ShadowMap *shadowMap;

	glm::mat4 lightProj; // projection matrix
};

