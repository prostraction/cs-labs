#pragma once
#include "Light.h"
#include <glm\gtc\matrix_transform.hpp>


class DirectionalLight :
    public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
                    GLfloat red, GLfloat green, GLfloat blue,
                    GLfloat aIntensity, GLfloat dIntensity,
                    GLfloat xDirection, GLfloat yDirection, GLfloat zDirection);

    void UseLight(GLuint ambientIntensityLocation, GLuint ColorLocation,
        GLuint diffuseIntensityLocation, GLuint directionLocation);

    glm::mat4 CalculateLightTransform();

    ~DirectionalLight();
private:
    glm::vec3 direction;
};

