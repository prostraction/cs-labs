#include "DirectionalLight.h"



DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f); // straight down
	//lightProj = glm::ortho(-5.0f, 5.0f, 5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
								GLfloat red, GLfloat green, GLfloat blue,
								GLfloat aIntensity, GLfloat dIntensity,
								GLfloat xDirection, GLfloat yDirection, GLfloat zDirection) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDirection, yDirection, zDirection);
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}
DirectionalLight::~DirectionalLight()
{

}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	// the light coming from the opposite direction
	// -direction = "fake" a direction
	// we are going from me opposite direction to the origin
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}