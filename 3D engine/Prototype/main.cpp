#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <cstdlib>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader_Loader.h"
#include "Triangle.h"
#include "Square.h"
#include "Init.h"
#include "TextureLoader.h"
#include "TextRenderer.h"

using namespace std;
using namespace Core;
using namespace Shapes;

bool showFPS = false;
bool showWireframe = false;
float yVelocity = 0;
float xVelocity = 0;
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, 1);
		if (key == GLFW_KEY_F)
			showFPS = !showFPS;
		if (key == GLFW_KEY_UP)
			yVelocity = 0.0f;
		if (key == GLFW_KEY_DOWN)
			yVelocity = 0.0f;
		if (key == GLFW_KEY_LEFT)
			xVelocity = 0.0f;
		if (key == GLFW_KEY_RIGHT)
			xVelocity = 0.0f;
		if (key == GLFW_KEY_W)
			showWireframe = !showWireframe;
	}

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_UP)
			yVelocity = 0.05f;
		if (key == GLFW_KEY_DOWN)
			yVelocity = -0.05f;
		if (key == GLFW_KEY_LEFT)
			xVelocity = 0.05f;
		if (key == GLFW_KEY_RIGHT)
			xVelocity = -0.05f;
	}
}
void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	
}
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{}


// Off-screen framebuffer definition
GLuint frameBufferObject;
GLuint frameBufferTexture;
GLuint renderBufferObject;
void GenerateOffscreenBuffer(int width, int height)
{
	int fullWidth = 2 * width;
	glGenTextures(1, &frameBufferTexture);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fullWidth, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &renderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fullWidth, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		cout << "Framebuffer completed" << endl;
}

// Vertices for window quads
GLuint leftScreenVBO;
GLuint rightScreenVBO;
void GenerateWindowQuads()
{
	GLfloat leftScreenVertices[] = {
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  0.5f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  0.5f, 0.0f,
		1.0f,  1.0f,  0.5f, 1.0f
	};
	GLfloat rightScreenVertices[] = {
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.5f, 1.0f,
		-1.0f, -1.0f,  0.5f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.5f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	glGenBuffers(1, &leftScreenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, leftScreenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftScreenVertices), leftScreenVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &rightScreenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rightScreenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightScreenVertices), rightScreenVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
int main()
{
	InitGlfw initGlfw;
	ShaderLoader shaderLoader;
	TextureLoader textureLoader;
	TextRenderer textRenderer;

	// Init
	initGlfw.Init();
	textRenderer.Init();
	glfwSetKeyCallback(initGlfw.window, KeyCallback);
	glfwSetMouseButtonCallback(initGlfw.window, MouseButtonCallback);
	glfwSetCursorPosCallback(initGlfw.window, MouseMoveCallback);

	// Shader loading
	GLuint defaultShaderProgram;
	GLuint textShaderProgram;
	GLuint simpleShaderProgram;
	defaultShaderProgram = shaderLoader.CreateProgram("Shaders\\vertex_shader.glsl", "Shaders\\fragment_shader.glsl");
	textShaderProgram = shaderLoader.CreateProgram("Shaders\\text_vs.glsl", "Shaders\\text_fs.glsl");
	simpleShaderProgram = shaderLoader.CreateProgram("Shaders\\simple_vertex_shader.glsl", "Shaders\\simple_fragment_shader.glsl");
	

	// Load texture
	GLuint texture = textureLoader.LoadRGB("Images\\container.jpg");
	GLuint blankTexture = textureLoader.LoadBlank();
	cout << "Blank texture status: " << blankTexture << endl;

	// Define camera component vectors
	float wallDistanceMargin = 0.2f;
	float currentX;
	float currentZ;

	glm::vec3 cameraPos = glm::vec3(0.0f, -0.5f, 6.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = -90.0f;
	glm::vec3 front(cos(glm::radians(yaw)), 0.0f, sin(glm::radians(yaw)));
	
	// Define model and view matrices and get shader locations
	GLuint modelLocation = glGetUniformLocation(defaultShaderProgram, "model");
	GLuint viewLocation = glGetUniformLocation(defaultShaderProgram, "view");
	GLuint projectionLocation = glGetUniformLocation(defaultShaderProgram, "projection");

	double currentTime = glfwGetTime();
	double delta;

	glm::mat4 view;
	glm::mat4 proj1;
	glm::mat4 idm  = glm::mat4();

	// Hard-coded virtual environment
	Square backWall;
	Square leftWall;
	Square rightWall;
	Square floorSquare;
	Square frontWall;
	Square rewardZone;
	float corridorDepth = 50.0f;
	float corridorWidth = 3.0f;
	float wallHeight = 2.0f;

	float rewardZoneDepth = 2.0f;
	float rewardZonePosition = -3.0f;

	float rewardZoneLowZ = rewardZonePosition - rewardZoneDepth;
	float rewardZoneHighZ = rewardZonePosition + rewardZoneDepth;
	bool inRewardZone = false;

	backWall.SetColor(0.5f, 0.5f, 0.9f);
	backWall.SetScaling(corridorWidth, wallHeight);
	backWall.SetPosition(0.0f, wallHeight / 2, -corridorDepth);

	frontWall.SetColor(0.5f, 0.5f, 0.9f);
	frontWall.SetScaling(corridorWidth, wallHeight);
	frontWall.SetPosition(0.0f, wallHeight / 2.0f, corridorDepth);

	leftWall.SetColor(1.0f, 1.0f, 1.0f);
	leftWall.SetScaling(corridorDepth, wallHeight);
	//leftWall.MatchTextureToScale();
	leftWall.SetRotation(0.0f, 90.0f, 0.0f);
	leftWall.SetPosition(-corridorWidth, wallHeight / 2, 0.0f);

	rightWall.SetColor(1.0f, 1.0f, 1.0f);
	rightWall.SetScaling(corridorDepth, wallHeight);
	//rightWall.MatchTextureToScale();
	rightWall.SetRotation(0.0f, 90.0f, 0.0f);
	rightWall.SetPosition(corridorWidth, wallHeight / 2, 0.0f);

	floorSquare.SetColor(1.0f, 1.0f, 1.0f);
	floorSquare.SetScaling(corridorWidth, corridorDepth);
	floorSquare.MatchTextureToScale();
	floorSquare.SetRotation(-90.0f, 0.0f, 0.0f);
	floorSquare.SetPosition(0.0f, -wallHeight / 2.0f, 0.0f);

	rewardZone.SetColor(0.2f, 0.9f, 0.2f);
	rewardZone.SetScaling(corridorWidth, rewardZoneDepth);
	rewardZone.SetRotation(-90.0f, 0.0f, 0.0f);
	rewardZone.SetPosition(0.0f, -wallHeight / 2.0f + 0.01f, rewardZonePosition);

	proj1 = glm::perspective(45.0f, (float)2.0f*initGlfw.windowInfo.width / initGlfw.windowInfo.height, 0.1f, 100.0f);

	GenerateOffscreenBuffer(initGlfw.windowInfo.width, initGlfw.windowInfo.height);
	GenerateWindowQuads();


	GLuint noiseTexture = textureLoader.LoadNoiseTexture(512, 512, 128, 7);
	GLuint gratingTexture = textureLoader.LoadGratingTexture(512, 512, 100, 0.02, 0.0);

	// Game loop
	int updateTick = 0;
	while (!glfwWindowShouldClose(initGlfw.window))
	{
		// Get time
		delta = glfwGetTime() - currentTime;
		currentTime = glfwGetTime();
		std::string fpsString = "FPS: " + std::to_string(delta);

		// Update view matrix
		// sensor readings need to be plugged in here
		front = glm::vec3(cos(glm::radians(yaw)), 0.0f, sin(glm::radians(yaw)));
		currentX = cameraPos.x;
		currentZ = cameraPos.z;

		cameraPos += yVelocity * front;
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * xVelocity;

		// Collision detection, reset to previous position if wall boundary is crossed
		if ( ((cameraPos.x-wallDistanceMargin) < -corridorWidth) | ((cameraPos.x + wallDistanceMargin)> corridorWidth))
			cameraPos.x = currentX;
		if (((cameraPos.z-wallDistanceMargin) < -corridorDepth) | ((cameraPos.z + wallDistanceMargin) > corridorDepth))
			cameraPos.z = currentZ;

		// Check reward zone entries
		if (!inRewardZone && (cameraPos.z > rewardZoneLowZ && cameraPos.z < rewardZoneHighZ))
		{
			inRewardZone = true;
			cout << "Reward zone ENTRY detected." << endl;
		}
		if (inRewardZone && (cameraPos.z < rewardZoneLowZ || cameraPos.z > rewardZoneHighZ))
		{
			inRewardZone = false;
			cout << "Reward zone EXIT detected." << endl;
		}

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		// Draw the complete screen to the offscreen framebuffer
		glfwMakeContextCurrent(initGlfw.window);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
		glViewport(0, 0, 2*initGlfw.windowInfo.width, initGlfw.windowInfo.height);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			if (showFPS)
				textRenderer.RenderText(textShaderProgram, fpsString, 0.0f, GLfloat(initGlfw.windowInfo.height - 20), 1.0, glm::vec3(0.8, 0.6, 0.6));

			glUseProgram(defaultShaderProgram);
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj1));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(idm));
			glBindTexture(GL_TEXTURE_2D, texture);

			frontWall.Draw();
			backWall.Draw();
			

			glBindTexture(GL_TEXTURE_2D, gratingTexture);
			leftWall.Draw();
			rightWall.Draw();

			glBindTexture(GL_TEXTURE_2D, noiseTexture);
			floorSquare.Draw();
			glBindTexture(GL_TEXTURE_2D, blankTexture);
			rewardZone.Draw();
			glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// First monitor drawing calls
		//glfwMakeContextCurrent(initGlfw.window);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glViewport(0, 0, initGlfw.windowInfo.width, initGlfw.windowInfo.height);
		glUseProgram(simpleShaderProgram);
		glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
		glBindBuffer(GL_ARRAY_BUFFER, rightScreenVBO);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glfwSwapBuffers(initGlfw.window);

		// Second monitor drawing calls
		glfwMakeContextCurrent(initGlfw.window2);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(simpleShaderProgram);
		glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
		glBindBuffer(GL_ARRAY_BUFFER, leftScreenVBO);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glfwSwapBuffers(initGlfw.window2);

		// Check events
		glfwPollEvents();

		// Output information
		++updateTick;
		if (updateTick % 60 == 0)
		{
			cout << "Camera position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << endl;
			updateTick = 0;
		}
	}
	glfwTerminate();

	// Clean up after exiting
	glDeleteFramebuffers(1, &frameBufferObject);
	return 0;
}