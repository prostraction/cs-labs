#include "Window.h"

/* Public section */
Window::Window()
{
	width = 800;
	height = 600;
	bufferWidth = 0;
	bufferHeight = 0;
	for (size_t i = 0; i < 1024; i++) {
		keys[i] = false;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;
	for (size_t i = 0; i < 1024; i++) {
		keys[i] = false;
	}
}
Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
int Window::Initialize() {
	// initialize GLFW
	if (!glfwInit()) {
		printf("GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}
	// setup GLFW window properties
	// openGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// core profile = no backward compability
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// allow forward compability
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}
	// get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// handle key and mouse input
	createCallback();

	// more for games = no cursor
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// allow modern extentios featues
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);	///////////////////////////////

	// setup Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//glfwSetCursorPos(mainWindow, (double) width / 2, (double) height / 2);
	glfwSetWindowUserPointer(mainWindow, this); // this = owner of class
}
void Window::swapBuffers() {
	glfwSwapBuffers(mainWindow);
}

bool* Window::getKeys() { 
	return keys; 
}
GLfloat Window::getXChange() {
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}
GLfloat Window::getYChange() {
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}
GLint Window::getBufferWidth() { 
	return bufferWidth; 
}
GLint Window::getBufferHeight() { 
	return bufferHeight; 
}
bool Window::getShouldClose() { 
	return glfwWindowShouldClose(mainWindow); 
}

/* Private section */
void Window::createCallback() {
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}
/* Static functions */
void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!theWindow)
	{
		;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("Pressed: %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("Released: %d\n", key);
		}
	}
}
void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!theWindow)
	{
		;
	}
	if (!theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = true;
		//return;
	}
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;	// NOT inverted vertical control (upside-down)

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	//printf("x: %.6f, y: %.6f. mouse first moved: %d\n", theWindow->xChange, theWindow->yChange, theWindow-> mouseFirstMoved);
}