#define STB_IMAGE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include <vector>
#include "Mesh.h"
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
#include "ShadowMap.h"


const float toRadians = 3.14159265f / 180.0f;

GLuint uniformProjection = 0, uniformModel = 0;
GLuint uniformView = 0;
GLuint uniformEyePosition = 0;
GLuint uniformSpecularIntensity = 0;
GLuint uniformShininess = 0;

Window* mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Shader directionalShadowShader;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

Model xwing;
Model blackhawk;

// delta time
GLfloat deltaTime	= 0.0f;
GLfloat lastTime	= 0.0f;
GLfloat currentTime	= 0.0f;

GLfloat blackhawkAngle = 0.0f;


bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.8f;
float triIncrement = 0.005f;
float angle = 0.0f;


// Vertex Shader
// view model is where camera positioned
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

const char* brickTextureLocation	= "Textures/IMG_0572.png";
const char* dirtTextureLocation		= "Textures/IMG_1569.bmp";
const char* plainTextureLocation	= "Textures/plain.png";

void calculateAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vertLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vertLength;		// indices x; 3 * 8 = 24 (third vertice)
		unsigned int in1 = indices[i + 1] * vertLength; // indices y
		unsigned int in2 = indices[i + 2] * vertLength; // indices z
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]); // x, y, z
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;	// nx
		in1 += normalOffset;	// ny
		in2 += normalOffset;	// nz
		vertices[in0] += normal.x;	vertices[in0 + 1] += normal.y;	vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x;	vertices[in1 + 1] += normal.y;	vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x;	vertices[in2 + 1] += normal.y;	vertices[in2 + 2] += normal.z;
	}
	for (size_t i = 0; i < verticeCount / vertLength; i++)
	{
		unsigned int nOffset = i * vertLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;	vertices[nOffset + 1] = vec.y;	vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2,
	};

	GLfloat vertices[] = {
		//	  x	     y	   z	  u		v		 nx	   ny	 nz	
			-1.0f, -1.0f, -0.5f,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 			// after this line, you cound set a color for this vertice and set it up on the AttribPointer
			0.0f, -1.0f, 1.0f,	 0.5f, 0.0f,	0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.5f,	 1.0f, 0.0f,	0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,	 0.5f, 1.0f,	0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	calculateAverageNormals(indices, 12, vertices, 32, 8, 5); // normal offset = path to nx


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFile(vShader, fShader);
	shaderList.push_back(shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFile("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
}

void RenderScene()
{
	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-7.0f, -0.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	xwing.RenderModel();

	blackhawkAngle -= 0.1f;
	if (blackhawkAngle < -360.0f)
		blackhawkAngle = 0.1f;

	model = glm::mat4(1.0f);
	model = glm::rotate(model, 4 * blackhawkAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-8.0f, 2.0f, 0.0f));
	model = glm::rotate(model, -20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	blackhawk.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();
	// set viewport to the same dimenstions as the framebuffer
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	// write to framebuffer
	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	glm::mat4 temp = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&temp);

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	shaderList[0]->UseShader();

	uniformModel				= shaderList[0]->GetModelLocation();
	uniformProjection			= shaderList[0]->GetProjectionLocation();
	uniformView					= shaderList[0]->GetViewLocation();
	uniformEyePosition			= shaderList[0]->GetEyePositionLocation();
	uniformSpecularIntensity	= shaderList[0]->GetSpecularIntensityLocation();
	uniformShininess			= shaderList[0]->GetShininessLocation();

	glViewport(0, 0, 1440, 1080);
	// Clear the window
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(uniformProjection,	1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView,			1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shaderList[0]->SetDirectionalLight(&mainLight);
	shaderList[0]->SetPointLights(pointLights, pointLightCount);
	shaderList[0]->SetSpotLights(spotLights, spotLightCount);
	glm::mat4 temp = mainLight.CalculateLightTransform();
	shaderList[0]->SetDirectionalLightTransform(&temp);

	mainLight.GetShadowMap()->Read(GL_TEXTURE1);	// texture init 1 in frag shader
	shaderList[0]->SetTexture(0);					// texture unit 0
	shaderList[0]->SetDirectionalShadowMap(1);		// texture unit 1

	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	RenderScene();
}

int main(int argc, char* argv[])
{
	mainWindow = new Window(1440, 1080);
	mainWindow->Initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f);

	brickTexture	= Texture(brickTextureLocation);		brickTexture.LoadTextureA();
	dirtTexture		= Texture(dirtTextureLocation);			dirtTexture.LoadTextureA();
	plainTexture	= Texture(plainTextureLocation);		plainTexture.LoadTextureA();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	xwing = Model();		xwing.LoadModel		("Models/x-wing.obj");
	blackhawk = Model();	blackhawk.LoadModel	("Models/uh60.obj");


	mainLight = DirectionalLight(2048, 2048,
		1.0f, 1.0f, 1.0f,
		0.15f, 0.5f,
		0.0f, -7.0f, -1.0f);



	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
		0.0f, 0.1f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
		0.0f, 0.1f,
		-4.0f, 2.0f, 0.0f,
		0.3f, 0.1f, 0.1f);
	pointLightCount++;

	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-100.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;



	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow->getBufferWidth() / mainWindow->getBufferHeight(), 0.1f, 100.0f);
	//projection = glm::translate(projection, glm::vec3(0.0f, -0.2f, 0.0f));

	GLint nTexSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &nTexSize);
	printf("%d\n\n\n", nTexSize);

	// Loop until window closed
	while (!mainWindow->getShouldClose())
	{
		currentTime = glfwGetTime();		// SDL_GetPerfomanceCounter();
		deltaTime = currentTime - lastTime;	// (currentTime - lastTime) * 1000 / SDL_GetPerfomanceFrequency(); == seconds [miliseconds * 1000]
		lastTime = currentTime;

		// Get + Handle User Input
		glfwPollEvents();
		camera.keyControl(mainWindow->getKeys(), deltaTime);
		camera.mouseControl(mainWindow->getXChange(), mainWindow->getYChange());

		DirectionalShadowMapPass(&mainLight);
		RenderPass(projection, camera.calculateViewMatrix());

		glUseProgram(0);
		mainWindow->swapBuffers();
	}

	for (size_t i = 0; i < shaderList.size(); i++)
	{
		delete shaderList[i];
	}

	for (size_t i = 0; i < meshList.size(); i++)
	{
		delete meshList[i];
	}

	shaderList.clear();
	meshList.clear();
	delete mainWindow; mainWindow = nullptr;

	return 0;
}