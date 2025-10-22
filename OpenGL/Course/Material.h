#pragma once

#include <GL\glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);
	~Material();

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

private:
	GLfloat specularIntensity;	// how much specular light should be on an object
	GLfloat shininess;			// how smooth will specular light will be (specular power)
};

