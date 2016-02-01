#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader_Loader.h"
#include "Triangle.h"
#include "Square.h"
#include "Init.h"
#include "TextureLoader.h"
#include "TextRenderer.h"
#include "RewardDelivery.h"
#include "MotionReader.h"
#include "ScreenManager.h"
using namespace std;
using namespace Core;
using namespace Shapes;

bool showFPS = false;
bool showWireframe = false;
bool applyDrift = false;
bool blockOnMotion = true;

float yVelocity = 0.0f;
float xVelocity = 0.0f;
float textureVelocity = -0.05f;
float cameraVelocity = 0.09f;
float cameraRotation = 0.0f;

MotionReader motionReader;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_L)
			cout << "Input bytes pending: " << motionReader.InputBytesPending();
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, 1);
		if (key == GLFW_KEY_F)
			showFPS = !showFPS;
		if (key == GLFW_KEY_UP)
		{
			yVelocity = 0.0f;
			textureVelocity = -0.02f;
		}
		if (key == GLFW_KEY_DOWN)
		{
			yVelocity = 0.0f;
			textureVelocity = -0.02f;
		}
		if (key == GLFW_KEY_LEFT)
			xVelocity = 0.0f;
		if (key == GLFW_KEY_RIGHT)
			xVelocity = 0.0f;
		if (key == GLFW_KEY_W)
			showWireframe = !showWireframe;
		if (key == GLFW_KEY_D)
			applyDrift = !applyDrift;
		if (key == GLFW_KEY_B)
		{
			blockOnMotion = !blockOnMotion;
			if (blockOnMotion)
				cout << "Blocking drift when moving" << endl;
			else
				cout << "Blocking drift when moving disabled" << endl;
		}
		if (key == GLFW_KEY_A)
		{
			cameraRotation += 2.0f;
			cout << "New camera Rotation: " << cameraRotation << endl;
		}
		if (key == GLFW_KEY_Z)
			cameraRotation -= 2.0f;
	}

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_UP)
		{
			yVelocity = cameraVelocity;
			if (blockOnMotion)
				textureVelocity = -0.00f;
		}
		if (key == GLFW_KEY_DOWN)
		{
			
			yVelocity = -cameraVelocity;
			if (blockOnMotion)
				textureVelocity = 0.00f;
		}
		if (key == GLFW_KEY_LEFT)
			xVelocity = cameraVelocity;
		
		if (key == GLFW_KEY_RIGHT)
			xVelocity = -cameraVelocity;
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
	ScreenManager screenManager;
	RewardDelivery rewardDelivery;
	
	// Initiate components
	initGlfw.Init();
	textRenderer.Init();
	screenManager.Initialize(initGlfw.windowInfo.width, initGlfw.windowInfo.height);	

	rewardDelivery.Initialize(0, 2);
	rewardDelivery.SetRewardDuration(150);
	motionReader.Connect(8);
	motionReader.SetParameters(40, 40, 31, 31, 200);
	
	// Set callback functions
	glfwSetKeyCallback(initGlfw.window, KeyCallback);
	glfwSetMouseButtonCallback(initGlfw.window, MouseButtonCallback);
	glfwSetCursorPosCallback(initGlfw.window, MouseMoveCallback);

	// Load shader programs
	GLuint defaultShaderProgram;
	GLuint textShaderProgram;
	GLuint simpleShaderProgram;
	defaultShaderProgram = shaderLoader.CreateProgram("Shaders\\vertex_shader.glsl", "Shaders\\fragment_shader.glsl");
	textShaderProgram = shaderLoader.CreateProgram("Shaders\\text_vs.glsl", "Shaders\\text_fs.glsl");
	simpleShaderProgram = shaderLoader.CreateProgram("Shaders\\simple_vertex_shader.glsl", "Shaders\\simple_fragment_shader.glsl");
	
	// Load textures
	GLuint texture = textureLoader.LoadRGB("Images\\container.jpg");
	GLuint blankTexture = textureLoader.LoadBlank();
	GLuint noiseTexture = textureLoader.LoadNoiseTexture(512, 512, 128, 7);
	GLuint gratingTexture = textureLoader.LoadGratingTexture(512, 512, 100, 0.02, 0.0);

	// Define camera component vectors
	float wallDistanceMargin = 0.3f;
	float currentX;
	float currentZ;

	glm::vec3 cameraPos = glm::vec3(0.0f, -0.5f, 80.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = -90.0f;
	glm::vec3 front(cos(glm::radians(yaw)), 0.0f, sin(glm::radians(yaw)));
	
	// Define model and view matrices and get shader locations
	GLuint modelLocation = glGetUniformLocation(defaultShaderProgram, "model");
	GLuint viewLocation = glGetUniformLocation(defaultShaderProgram, "view");
	GLuint projectionLocation = glGetUniformLocation(defaultShaderProgram, "projection");

	GLuint textureOffsetLocation = glGetUniformLocation(defaultShaderProgram, "textureOffset");

	double currentTime = glfwGetTime();
	double delta;

	glm::mat4 view;
	glm::mat4 proj1;
	glm::mat4 idm  = glm::mat4();
	glm::mat4 camera1;
	glm::mat4 camera2;

	// Hard-coded virtual environment
	Square backWall;
	Square leftWall;
	Square rightWall;
	Square floorSquare;
	Square frontWall;
	Square rewardZone;
	Square rewardZone2;

	float corridorDepth = 100.0f;
	float corridorWidth = 3.0f;
	float wallHeight = 2.0f;

	float rewardZoneDepth = 2.0f;
	float rewardZonePosition1 = 70.0f;
	float rewardZonePosition2 = 50;

	float rewardZoneLowZ = rewardZonePosition1 - rewardZoneDepth;
	float rewardZoneHighZ = rewardZonePosition1 + rewardZoneDepth;
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
	rewardZone.SetPosition(0.0f, -wallHeight / 2.0f + 0.01f, rewardZonePosition1);

	rewardZone2.SetColor(0.2f, 0.9f, 0.2f);
	rewardZone2.SetScaling(corridorWidth, rewardZoneDepth);
	rewardZone2.SetRotation(-90.0f, 0.0f, 0.0f);
	rewardZone2.SetPosition(0.0f, -wallHeight / 2.0f + 0.01f, rewardZonePosition2);

	if (initGlfw.twinCameraMode)
		proj1 = glm::perspective(45.0f, (float)initGlfw.windowInfo.width / initGlfw.windowInfo.height, 0.1f, 100.0f);
	else
		proj1 = glm::perspective(45.0f, (float)2.0*initGlfw.windowInfo.width / initGlfw.windowInfo.height, 0.1f, 100.0f);

	float textureOffset = 0.0;
	float timeInRewardZone = 0.0f;

	// Game loop
	int updateTick = 0;
	int rewardZonePasses = 0;

	motionReader.StartReading();
	double xMotionReader, yMotionReader;
	rewardDelivery.StartReward();

	ofstream logFile;
	logFile.open("C:\\VR_SYSTEM\\Data\\3D_pilot\\20160128\\68680_R3.dat", ios::binary | ios::out);

	while (!glfwWindowShouldClose(initGlfw.window))
	{
		// Get time
		delta = glfwGetTime() - currentTime;
		currentTime = glfwGetTime();

		motionReader.PollSensor(xMotionReader, yMotionReader);
		xMotionReader = -xMotionReader/(100.0f*delta);

		if (applyDrift)
		{
		textureOffset -= (textureVelocity*(float)delta);
		if (textureOffset < -1.0)
			textureOffset += 1.0;
		}
		std::string fpsString = "FPS: " + std::to_string(delta) +"\nxVel: " + std::to_string(xMotionReader);

		// Update view matrix
		// Code for rotating about central axis
		//yaw += xVelocity;
		front.x = cos(glm::radians(yaw));
		front.y = 0.0f;
		front.z = sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);


		currentX = cameraPos.x;
		currentZ = cameraPos.z;

		//cameraPos += yVelocity * cameraFront;
		cameraPos += (float)xMotionReader*cameraFront;
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * xVelocity;

		
		// Collision detection, reset to previous position if wall boundary is crossed
		glfwMakeContextCurrent(initGlfw.window);
		if ( ((cameraPos.x-wallDistanceMargin) < -corridorWidth) | ((cameraPos.x + wallDistanceMargin)> corridorWidth))
			cameraPos.x = currentX;
		if (((cameraPos.z-wallDistanceMargin) < -corridorDepth) | ((cameraPos.z + wallDistanceMargin) > corridorDepth))
			cameraPos.z = currentZ;
		if (cameraPos.z < 60)
		{
			textureOffset += 0.1f; // Allows for smooth texture flow
			cameraPos.z += 20.0f;
			if (rewardZonePasses % 2 == 0)
			{
				rewardZone.SetColor(0.2f, 0.9f, 0.2f);
				rewardZone2.SetColor(0.2f, 0.9f, 0.2f);
			}
			else
			{
				rewardZone.SetColor(0.2f, 0.9f, 0.2f);
				rewardZone2.SetColor(0.2f, 0.9f, 0.2f);
			}
			++rewardZonePasses;
			cout << "Reward zone pas: " << rewardZonePasses << endl;
		}

		// Check reward zone entries
		if (inRewardZone)
		{
			timeInRewardZone += (float)delta;
		}
		if (!inRewardZone && (cameraPos.z > rewardZoneLowZ && cameraPos.z < rewardZoneHighZ))
		{
			inRewardZone = true;
			timeInRewardZone = 0.0;
			rewardDelivery.PulseReward();
			cout << "Reward zone ENTRY detected." << endl;
		}
		if (inRewardZone && (cameraPos.z < rewardZoneLowZ || cameraPos.z > rewardZoneHighZ))
		{
			inRewardZone = false;
			cout << "Reward zone EXIT detected." << endl;
			cout << "Total time in reward zone: " << timeInRewardZone << endl;
		}

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// Write data to logfile
		logFile.write((char*)&currentTime, sizeof(double));
		logFile.write((char*)&xMotionReader, sizeof(double));
		logFile.write((char*)&cameraPos.z, sizeof(float));
		logFile.write((char*)&inRewardZone, sizeof(bool));

		// Draw the complete screen to the offscreen framebuffer
		if (initGlfw.twinCameraMode)
		{ 
			// Global configurations
			//glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
			screenManager.DrawToTexture();
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_POLYGON_SMOOTH);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(initGlfw.windowInfo.width, 0, initGlfw.windowInfo.width, initGlfw.windowInfo.height);
			glUseProgram(defaultShaderProgram);
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj1));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(idm));

			// Left screen
			glm::vec3  front1 = glm::vec3(cos(glm::radians(yaw+cameraRotation)), 0.0f, sin(glm::radians(yaw+cameraRotation)));
			camera1 = glm::lookAt(cameraPos, cameraPos + front1, cameraUp);
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera1));
			glUniform1f(textureOffsetLocation, 0.0f);
			
			glBindTexture(GL_TEXTURE_2D, texture);
			frontWall.Draw();
			backWall.Draw();

			glBindTexture(GL_TEXTURE_2D, noiseTexture);
			floorSquare.Draw();
			
			glBindTexture(GL_TEXTURE_2D, blankTexture);
			rewardZone.Draw();
			rewardZone2.Draw();

			glBindTexture(GL_TEXTURE_2D, gratingTexture);
			glUniform1f(textureOffsetLocation, textureOffset);
			leftWall.Draw();
			rightWall.Draw();
			
			// Right screen
			glViewport(0, 0, initGlfw.windowInfo.width, initGlfw.windowInfo.height);
			glm::vec3 front2 = glm::vec3(cos(glm::radians(yaw - cameraRotation)), 0.0f, sin(glm::radians(yaw - cameraRotation)));
			camera2 = glm::lookAt(cameraPos, cameraPos + front2, cameraUp);
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera2));
			glUniform1f(textureOffsetLocation, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texture);
			frontWall.Draw();
			backWall.Draw();

			glBindTexture(GL_TEXTURE_2D, noiseTexture);
			floorSquare.Draw();

			glBindTexture(GL_TEXTURE_2D, blankTexture);
			rewardZone.Draw();
			rewardZone2.Draw();

			glBindTexture(GL_TEXTURE_2D, gratingTexture);
			glUniform1f(textureOffsetLocation, textureOffset);
			leftWall.Draw();
			rightWall.Draw();
			
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			//glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
			screenManager.DrawToTexture();
				glViewport(0, 0, 2 * initGlfw.windowInfo.width, initGlfw.windowInfo.height);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_POLYGON_SMOOTH);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				if (showFPS)
					textRenderer.RenderText(textShaderProgram, fpsString, 0.0f, GLfloat(initGlfw.windowInfo.height - 20), 1.0, glm::vec3(0.8, 0.6, 0.6));

				glUseProgram(defaultShaderProgram);
				glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj1));
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(idm));
				glUniform1f(textureOffsetLocation, 0.0f);
				glBindTexture(GL_TEXTURE_2D, texture);

				frontWall.Draw();
				//backWall.Draw();

				glBindTexture(GL_TEXTURE_2D, noiseTexture);
				floorSquare.Draw();
				glBindTexture(GL_TEXTURE_2D, blankTexture);
				rewardZone.Draw();
				rewardZone2.Draw();

				glBindTexture(GL_TEXTURE_2D, gratingTexture);
				glUniform1f(textureOffsetLocation, textureOffset);
				leftWall.Draw();
				rightWall.Draw();
				glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// First monitor drawing calls
		//glfwMakeContextCurrent(initGlfw.window);
		glfwMakeContextCurrent(initGlfw.window);
		glUseProgram(simpleShaderProgram);
		glViewport(0, 0, initGlfw.windowInfo.width, initGlfw.windowInfo.height);
		screenManager.DrawLeftTexture();
		glfwSwapBuffers(initGlfw.window);

		// Second monitor drawing calls
		glfwMakeContextCurrent(initGlfw.window2);
		glUseProgram(simpleShaderProgram);
		glViewport(0, 0, initGlfw.windowInfo.width, initGlfw.windowInfo.height);
		screenManager.DrawRightTexture();
		glfwSwapBuffers(initGlfw.window2);

		// Check events
		glfwPollEvents();


	}
	logFile.close();
	rewardDelivery.StopReward();
	motionReader.StopReading();
	glfwTerminate();
	return 0;
}