#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>

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
		std::cout << "Key release event detected: " << key << "," << scancode << endl;

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
			yVelocity = 0.03f;
		if (key == GLFW_KEY_DOWN)
			yVelocity = -0.03f;
		if (key == GLFW_KEY_LEFT)
			xVelocity = 0.03f;
		if (key == GLFW_KEY_RIGHT)
			xVelocity = -0.03f;
	}
}


void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	
}
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{}

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
	
	// Hard-coded virtual environment
	Square square;
	Square leftWall;
	Square rightWall;
	Square floorSquare;

	// Load texture
	// TO DO: create texture manager
	GLuint texture = textureLoader.LoadRGB("Images\\container.jpg");

	// Define camera component vectors
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 6.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = -90.0f;
	glm::vec3 front(cos(glm::radians(yaw)), 0.0f, sin(glm::radians(yaw)));
	
	// Define model and view matrices for each object
	// TO DO: clean up this code, add all view matrices to a single wallManager class
	GLuint modelLocation = glGetUniformLocation(defaultShaderProgram, "model");
	GLuint viewLocation = glGetUniformLocation(defaultShaderProgram, "view");
	GLuint projectionLocation = glGetUniformLocation(defaultShaderProgram, "projection");

	double currentTime = glfwGetTime();
	double delta;


	glm::mat4 model1;
	
	glm::mat4 leftWallModel;
	glm::mat4 floorModel;
	glm::mat4 rightWallModel;

	glm::mat4 view;
	glm::mat4 proj1;
	glm::mat4 idm  = glm::mat4();


	model1 = glm::translate(idm, glm::vec3(0.0f, 0.0f, -3.0f));

	square.SetPosition(-0.5f, 0.0f, 0.0f);
	square.SetRotation(0.0f, 0.0f, 0.0f);
	
	leftWall.SetScaling(2.0f, 1.0f);
	leftWall.SetRotation(0.0f, 90.0f, 0.0f);
	leftWall.SetPosition(-2.0f, 0.0f, 0.0f);
	
	rightWall.SetScaling(2.0f, 1.0f);
	rightWall.SetRotation(0.0f, 90.0f, 0.0f);
	rightWall.SetPosition(2.0f, 0.0f, 0.0f);
	
	floorSquare.SetColor(1.0f, 1.0f, 0.5f);
	floorSquare.SetScaling(2.0f, 2.0f);
	floorSquare.SetRotation(-90.0f, 0.0f, 0.0f);
	floorSquare.SetPosition(0.0f, -1.0f, 0.0f);

	/*	
	leftWallModel = glm::translate(idm, glm::vec3(-0.5f, -0.0f, 0.0f));
	leftWall.SetPosition(-0.5f, 0.0f, 0.0f);
	leftWall.SetScaling(1.0f, 3.0f);

	leftWallModel = glm::rotate(leftWallModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	rightWallModel = glm::translate(idm, glm::vec3(0.5f, 0.0f, 0.0f));
	rightWallModel = glm::rotate(rightWallModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	floorModel = glm::translate(idm, glm::vec3(0.0f, -1.0f, -1.0f));
	floorModel = glm::rotate(floorModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	*/
	proj1 = glm::perspective(45.0f, (float)2.0f*initGlfw.windowInfo.width / initGlfw.windowInfo.height, 0.1f, 100.0f);
	glEnable(GL_DEPTH_TEST);

	// Frame buffer test code: try to render to an offscreen framebuffer and copy relevant parts to each screen
	GLuint frameBufferObject;
	glGenFramebuffers(1, &frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		cout << "Framebuffer completed" << endl;
	
	// Off-screen texture rendering
	int fullWidth =  2*initGlfw.windowInfo.width;
	GLuint frameBufferTexture;
	glGenTextures(1, &frameBufferTexture);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fullWidth, initGlfw.windowInfo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		cout << "Framebuffer completed" << endl;

	// Off-screen renderbuffer for depth testing
	GLuint renderBufferObject;
	glGenRenderbuffers(1, &renderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fullWidth, initGlfw.windowInfo.height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		cout << "Framebuffer completed" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Generate quads for left and right window
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
	GLuint leftScreenVBO;
	GLuint rightScreenVBO;
	glGenBuffers(1, &leftScreenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, leftScreenVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(leftScreenVertices), leftScreenVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &rightScreenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rightScreenVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rightScreenVertices), rightScreenVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	
	// Game loop
	
	while (!glfwWindowShouldClose(initGlfw.window))
	{
		// Get time
		delta = glfwGetTime() - currentTime;
		currentTime = glfwGetTime();
		std::string fpsString = "FPS: " + std::to_string(delta);

		// Update view matrix
		// sensor readings need to be plugged in here
		front = glm::vec3(cos(glm::radians(yaw)), 0.0f, sin(glm::radians(yaw)));
		cameraPos += yVelocity * front;
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * xVelocity;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		// Draw the complete screen to the offscreen framebuffer
		glfwMakeContextCurrent(initGlfw.window);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
		glViewport(0, 0, fullWidth, initGlfw.windowInfo.height);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			

			// Show screen refresh rate
			if (showFPS)
				textRenderer.RenderText(textShaderProgram, fpsString, 0.0f, GLfloat(initGlfw.windowInfo.height - 20), 1.0, glm::vec3(0.8, 0.6, 0.6));

			// Draw environment objects
			glUseProgram(defaultShaderProgram);
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj1));
			glBindTexture(GL_TEXTURE_2D, texture);

				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(idm));
				//square.Draw();

				//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(leftWallModel));
				leftWall.Draw();

				//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(rightWallModel));
				rightWall.Draw();

				//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(floorModel));
				floorSquare.Draw();

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

		// Check events, collision detection happens here
		glfwPollEvents();
	}
	glfwTerminate();

	// Clean up after exiting
	glDeleteFramebuffers(1, &frameBufferObject);
	return 0;
}