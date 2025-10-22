#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <cstdio>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

	int Initialize();
	bool getShouldClose();
	void swapBuffers();

	GLint getBufferWidth();
	GLint getBufferHeight();

	bool* getKeys();
	GLfloat getXChange();
	GLfloat getYChange();

private:
	GLFWwindow* mainWindow = nullptr;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	
	bool keys[1024];
	void createCallback();

	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};