#pragma once

#include <GL\glew.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);
	~Texture();

	bool LoadTexture();
	bool LoadTextureA(); // with alpha

	void UseTexture();
	void ClearTexture();

private:
	GLuint textureID;
	int width, height, bitDepth;
	const char* fileLocation;
};

