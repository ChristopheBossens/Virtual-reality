#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <algorithm>

#include "Core.h"
#include "RewardDelivery.h"
#include "MotionReader.h"
#include "ScreenManager.h"
#include "SceneManager.h"
#include "ExperimentManager.h"

using namespace std;
using namespace Engine;
using namespace Shapes;

bool use2D = true;

float xDelta = 0.0f;
float zDelta = 0.0f;
double mouseX = 0.0, mouseY = 0.0;

// 2D experiment configuration variables
int flipFrames = 60; // stimulus presentation duration
bool isAlternating = false;
bool stimulusVisible = true;
bool increaseSize = false;
bool decreaseSize = false;
vector<int> gratingOrientations{ 0,15,30,45,60,75,90 };
int orientationIndex = 0;

// Global objects
MotionReader motionReader;
Engine::Core core;
RewardDelivery rewardDelivery;

// Input callback functions
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(core.baseWindow, 1);


		if (key == GLFW_KEY_A)
		{
			isAlternating = !isAlternating;
			if (isAlternating == false)
				stimulusVisible = true;
		}

		switch (key)
		{
		case GLFW_KEY_UP:
		case GLFW_KEY_DOWN:
			zDelta = 0.0f;
			break;

		case GLFW_KEY_LEFT:
		case GLFW_KEY_RIGHT:
			xDelta = 0.0f;
			break;
		}

	}

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_UP:
			zDelta = -0.01f;
			break;
		case GLFW_KEY_DOWN:
			zDelta = 0.01f;
			break;
		case GLFW_KEY_LEFT:
			xDelta = -0.01f;
			break;
		case GLFW_KEY_RIGHT:
			xDelta = 0.01f;
			break;
		}
	}
}
void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	mouseX = xpos;
	mouseY = ypos;
	if (window == core.secondWindow)
		mouseX += core.windowInfo.width;
}
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0.5)
		increaseSize = true;
	if (yoffset < -0.5)
		decreaseSize = true;
}
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		rewardDelivery.PulseReward();
}

void ConfigureCore()
{
	WindowInfo windowInfo;

	cout << endl;
	cout << "**************************" << endl;
	cout << "Environment configuration" << endl;
	cout << "**************************" << endl;
	cout << "i\tRetrieves a list of available screen modes." << endl;
	cout << "f\t Toggle fullscreen mode" << endl;
	cout << "t\t Toggle camera mode" << endl;
	cout << "w\t Set resolution width" << endl;
	cout << "h\t Set resolution height" << endl;
	cout << "v\t Toggle 2D/3D mode" << endl << endl;
	cout << "Press o to continue" << endl;

	char userInput;
	int number;
	while (true)
	{
		cin >> userInput;
		if ( (userInput == 'f') || (userInput == 'F'))
			windowInfo.useFullscreen = !windowInfo.useFullscreen;
		if ((userInput == 'w') || (userInput == 'W'))
		{
			cin >> number;
			windowInfo.width = number;
		}
		if ((userInput == 'h') || userInput == ('H'))
		{
			cin >> number;
			windowInfo.height = number;
		}
		if ((userInput == 'i') || userInput == ('I'))
		{
			core.PrintMonitorInfo();
		}
		if ((userInput == 'o') || (userInput == 'O'))
		{
			cout << "Starting engine" << endl;
			break;
		}
		if ((userInput == 'v') || (userInput == 'V'))
		{
			use2D = !use2D;
			if (use2D)
				cout << "2D mode enabled" << endl;
			else
				cout << "3D mode enabled" << endl;
		}
	}
	core.Initialize(windowInfo);
}
int main()
{
	TextureLoader textureLoader;
	ScreenManager screenManager;
	SceneManager corridor;
	ExperimentManager experimentManager;

	// Load configuration file
	ifstream configFile("CONFIG.txt", std::ifstream::in);
	int rewardDuration = 0;
	if (configFile.is_open())
	{
		cout << "File is open" << endl;
		configFile >> rewardDuration;
		configFile.close();
	}
	cout << "Retrieved reward duration: " << rewardDuration << endl;

	// Initiate components
	ConfigureCore();
	screenManager.Initialize(core.windowInfo.width, core.windowInfo.height, &core);	

	rewardDelivery.Initialize(0, 2);
	rewardDelivery.SetRewardDuration(150);
	motionReader.Connect(8);
	motionReader.SetParameters(40, 40, 31, 31, 200);
	
	
	corridor.LoadTextures(&textureLoader);
	corridor.LoadShaders();
	corridor.LoadMesh();
	corridor.SetProjectionMatrix(45.0f, 2.0f*core.windowInfo.width / core.windowInfo.height);

	experimentManager.SetOrthographicMatrix(0.0f, 2 * core.windowInfo.width, core.windowInfo.height, 0.0f);
	experimentManager.LoadTestExperiment(&textureLoader);

	// Set callback functions for keyboard and mouse devices
	glfwSetKeyCallback(core.baseWindow, KeyCallback);
	glfwSetKeyCallback(core.secondWindow, KeyCallback);

	glfwSetCursorPosCallback(core.baseWindow, MouseMoveCallback);
	glfwSetCursorPosCallback(core.secondWindow, MouseMoveCallback);

	glfwSetScrollCallback(core.baseWindow, MouseScrollCallback);
	glfwSetScrollCallback(core.secondWindow, MouseScrollCallback);

	glfwSetMouseButtonCallback(core.baseWindow, MouseButtonCallback);
	glfwSetMouseButtonCallback(core.secondWindow, MouseButtonCallback);

	glfwSetInputMode(core.baseWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(core.secondWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


	double currentTime = glfwGetTime();
	double delta;

	// Game loop
	unsigned int frameCount = 0;
	int updateTick = 0;

	motionReader.StartReading();
	double xMotionReader, yMotionReader;
	rewardDelivery.StartReward();

	ofstream logFile;
	
	//logFile.open("C:\\VR_SYSTEM\\Data\\3D_pilot\\20160128\\68680_R3.dat", ios::binary | ios::out);

	while (!glfwWindowShouldClose(core.baseWindow))
	{
		// Get time
		delta = glfwGetTime() - currentTime;
		currentTime = glfwGetTime();

		motionReader.PollSensor(xMotionReader, yMotionReader);
		xMotionReader = -xMotionReader/(100.0f*delta);

		
		// Write data to logfile
		/*logFile.write((char*)&currentTime, sizeof(double));
		logFile.write((char*)&xMotionReader, sizeof(double));
		logFile.write((char*)&cameraPos.z, sizeof(float));
		logFile.write((char*)&inRewardZone, sizeof(bool));*/

		// Draw the complete screen to the offscreen framebuffer
		screenManager.DrawToTexture();
		if (use2D)
		{
			experimentManager.Update(mouseX, mouseY);
			experimentManager.DrawExperiment();
		}
		else
		{
			corridor.UpdatePosition(xDelta, 0.0f, zDelta);
			corridor.Draw();
		}
		screenManager.DrawToScreen();

		// Update frame count
		++frameCount;

		// Check events
		glfwPollEvents();
	}

	// Clean up
	logFile.close();
	rewardDelivery.StopReward();
	motionReader.StopReading();

	glfwTerminate();

	cout << "Requested window resolution: " << endl;
	cout << core.windowInfo.width << " x " << core.windowInfo.height << endl;

	cout << "Obtained window resolution: " << endl;
	cout << core.baseWindowInfo.width << " x " << core.baseWindowInfo.height << endl;
	cout << core.secondWindowInfo.width << " x " << core.secondWindowInfo.width << endl;
	cin.get();
	system("pause");
	return 0;
}