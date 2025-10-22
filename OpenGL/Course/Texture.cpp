#include "Texture.h"

Texture::Texture() {
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = 0;
}
Texture::Texture(const char* fileLoc) {
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}
Texture::~Texture() {
	ClearTexture();
}

bool Texture::LoadTexture() {
	// load texture:
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}
	// bind texture:
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// how to handle wrapping on "s-axis" (x-axis)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		// how to handle wrapping on "t-axis" (y-axis)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// how to handle filter of the texture when it's made smaller (is father away)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// how to handle filter of the texture when it's made bigger  (is closer)

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
	// texture is loaded in memory, now we need to unbind it:
	glBindTexture(GL_TEXTURE_2D, 0);
	// free texture:
	stbi_image_free(texData);

	return true;
}

bool Texture::LoadTextureA() {
	// load texture:
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}
	// bind texture:
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// how to handle wrapping on "s-axis" (x-axis)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		// how to handle wrapping on "t-axis" (y-axis)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// how to handle filter of the texture when it's made smaller (is father away)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// how to handle filter of the texture when it's made bigger  (is closer)

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
	// texture is loaded in memory, now we need to unbind it:
	glBindTexture(GL_TEXTURE_2D, 0);
	// free texture:
	stbi_image_free(texData);

	return true;
}

void Texture::UseTexture() {
	glActiveTexture(GL_TEXTURE0);			 // 0 -- texture unit
	glBindTexture(GL_TEXTURE_2D, textureID); // bind texture to texture unit
}
void Texture::ClearTexture() {
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = height = bitDepth = 0;
	fileLocation = 0;
}