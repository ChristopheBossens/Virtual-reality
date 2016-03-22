#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>

#include "Core.h"
#include "RewardDelivery.h"
#include "MotionReader.h"
#include "ExperimentManager.h"
#include "DataLogger.h"
#include "TexturedCorridor.h"
#include "AppIO.h"

using namespace std;
using namespace Shapes;

bool use2D = true;

double xDelta = 0.0f;
double zDelta = 0.0f;

// Global objects
MotionReader motionReader;
Core core;
RewardDelivery rewardDelivery;
ExperimentManager experimentManager;
DataLogger dataLogger;
unsigned int dataPacket =0;

bool takeScreenshot = false;;
bool switchEnvironment = false;

// Temporary motion processing, will be moved to a separate class that allows to run 
// configuration files that specify how we should react to motion
enum State
{
	WAIT_FOR_MOTION,
	REWARD_FOR_MOTION,
	WAIT_FOR_STATIONARY,
	REWARD_FOR_STATIONARY
};
State currentState = WAIT_FOR_MOTION;
unsigned int runningDuration = 0;
unsigned int stationaryDuration = 0;
void ProcessMotion()
{
	// First
	static unsigned int stateDuration = 0;
	if (xDelta > AppIO::rewardParams.runningThresholdVelocity)
	{
		stationaryDuration = 0;
		++runningDuration;
	}
	else
	{
		runningDuration = 0;
		++stationaryDuration;
	}

	switch (currentState)
	{
	case WAIT_FOR_MOTION:
		if (runningDuration >= AppIO::rewardParams.runningThresholdDuration)
		{
			dataPacket = 1;
			rewardDelivery.PulseReward();
			runningDuration = 0;
			currentState = REWARD_FOR_MOTION;
		}
		break;
	case REWARD_FOR_MOTION:
		++stateDuration;
		if (stateDuration >= AppIO::rewardParams.rewardStateDuration)
		{
			stateDuration = 0;
			currentState = WAIT_FOR_STATIONARY;
		}
		if (runningDuration >= AppIO::rewardParams.runningThresholdDuration)
		{
			dataPacket = 1;
			rewardDelivery.PulseReward();
			runningDuration = 0;
		}
		
		break;
	case WAIT_FOR_STATIONARY:
		if (stationaryDuration >= AppIO::rewardParams.stationaryThresholdDuration)
		{
			dataPacket = 1;
			rewardDelivery.PulseReward();
			stationaryDuration = 0;
			currentState = REWARD_FOR_STATIONARY;
		}
		break;
	case REWARD_FOR_STATIONARY:
		++stateDuration;
		if (stateDuration >= AppIO::rewardParams.stationaryStateDuration)
		{
			stateDuration = 0;
			currentState = WAIT_FOR_MOTION;
		}
		if (runningDuration >= AppIO::rewardParams.stationaryThresholdDuration)
		{
			dataPacket = 1;
			rewardDelivery.PulseReward();
			stationaryDuration = 0;
		}
		break;
	}

	dataPacket += currentState << 4;
}

// Input callback functions
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			core.SetShouldClose();
			break;
		case GLFW_KEY_S:
			if (use2D)
			{
				experimentManager.keyStates[GLFW_KEY_S] = true;
			}
			break;
		case GLFW_KEY_D:
			experimentManager.keyStates[GLFW_KEY_D] = true;
			break;
		case GLFW_KEY_UP:
		case GLFW_KEY_DOWN:
			zDelta = 0.0f;
			break;

		case GLFW_KEY_T:
			switchEnvironment = true;
			break;
		case GLFW_KEY_LEFT:
		case GLFW_KEY_RIGHT:
			xDelta = 0.0f;
			break;

		case GLFW_KEY_P:
			takeScreenshot = true;
			break;

		case GLFW_KEY_R:
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
	experimentManager.mouseX = (int)xpos;
	experimentManager.mouseY = (int)ypos;
	experimentManager.mouseX += core.GetWindowOffset(window);
}
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	experimentManager.scrollDirection = (int)yoffset;
}
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		rewardDelivery.PulseReward();
}
void SetCallbacks()
{
	core.SetKeyboardCallback(KeyCallback);
	core.SetMouseMoveCallback(MouseMoveCallback);
	core.SetMouseScrollCallback(MouseScrollCallback);
	core.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

// Configuration interface for the data log file
void SetOutputFile()
{
	string dataPath = AppIO::CreateDataFolder();
	string filename;
	bool validFilename = false;
	getline(cin, filename);

	while (!validFilename)
	{
		filename = "";
		cout << "Enter a filename to save data: " << endl;
		getline(cin, filename);

		if (filename == "")
		{
			cout << "No filename specified. Data will not be saved." << endl;
			validFilename = true;
		}
		else
		{
			if (AppIO::FileExists(dataPath + filename))
			{
				cout << filename << " already exists. Choose a different filename." << endl;
			}
			else
			{
				cout << "Data will be saved to " << filename << endl;
				dataLogger.SetLogfile(dataPath + filename);
				validFilename = true;
			}
		}
	}
}
void ConfigureCore()
{
	cout << "Select an experiment: " << endl;
	cout << "1)\t2D training" << endl;
	cout << "2)\t2D receptive field mapping" << endl;
	cout << "3)\t2D orientation tuning" << endl;
	cout << "4)\t3D training" << endl;
	
	string userInput;
	bool useFullscreen = true;

	AppIO::ReadConfiguration();
	while (true)
	{
		cin >> userInput;
		if (userInput[0]== '1')
		{
			use2D = true;
			SetOutputFile();
			break;
		}
		if (userInput[0]=='4')
		{
			use2D = false;
			SetOutputFile();
			break;
		}
	}

	if (userInput.length() >= 2)
	{
		if (userInput[1] == 'd')
			useFullscreen = false;
	}

	rewardDelivery.Initialize(AppIO::appParams.rewardPort, AppIO::appParams.rewardSwitch);
	rewardDelivery.SetRewardDuration(AppIO::appParams.rewardDuration);
	motionReader.Connect(AppIO::appParams.motionPort);
	motionReader.SetParameters(40, 40, 31, 31, 200);
	core.Initialize(AppIO::appParams.screenHeight,AppIO::appParams.screenWidth,2,useFullscreen);
}


int main()
{
	TextureCorridor textureCorridor;

	// Initiate components
	ConfigureCore();
	SetCallbacks();
	
	
	int currentEnvironment = 1;
	if (use2D)
	{
		experimentManager.Initialize();
		experimentManager.SetOrthographicMatrix(0.0f, (float)core.GetBufferWidth(), (float)core.GetBufferHeight(), 0.0f);
		//experimentManager.LoadExperimentXML("C:\\Users\\Christophe\\Documents\\GitHub\\Virtual-reality\\3D engine\\Debug\\Experiments\\RF_Position.xml");
		//experimentManager.InitiateExperiment();
		experimentManager.InitiateSandbox();
	}
	else
	{
		textureCorridor.LoadTextures();
		textureCorridor.LoadShaders();
		textureCorridor.LoadMesh(currentEnvironment);
		/*
		corridor.LoadTextures(&textureLoader);
		corridor.LoadShaders();
		corridor.LoadMesh();
		corridor.SetProjectionMatrix(45.0f, (float)core.GetScreenWidth() / core.GetScreenHeight());
		*/

	}

	double currentTime = glfwGetTime();
	double timeDelta;

	// Game loop
	unsigned int frameCount = 0;

	motionReader.StartReading();
	rewardDelivery.StartReward();
	dataLogger.StartLogging();
	while (!core.ShouldClose())
	{
		// Get motion input (output of PollSensor gives displacement in cm)
		timeDelta = glfwGetTime() - currentTime;
		currentTime = glfwGetTime();
		motionReader.PollSensor(zDelta, xDelta);


		// Update the experiments
		if (use2D)
		{
			if (experimentManager.IsRunning())
			{
				experimentManager.ProcessInput();
				experimentManager.UpdateSandbox(frameCount);
				experimentManager.DrawSandbox();
			}
			else
			{
				glClear(GL_COLOR_BUFFER_BIT);
			}
		}
		else
		{
			ProcessMotion();
			if (switchEnvironment)
			{
				switchEnvironment = false;
				if (currentEnvironment == 1)
					currentEnvironment = 2;
				else
					currentEnvironment = 1;
				textureCorridor.LoadMesh(currentEnvironment);
			}
			
			textureCorridor.UpdatePosition(0.0f, 0.0f, (float)zDelta);
			textureCorridor.Draw();
		}

		if (takeScreenshot)
		{
			takeScreenshot = false;
		}
		core.RenderFramebuffer();

		// Update framecount and log all data to file
		dataLogger.LogData(frameCount);
		dataLogger.LogData(timeDelta);
		dataLogger.LogData(zDelta);
		dataLogger.LogData(xDelta);
		dataLogger.LogData(dataPacket);
		dataPacket = 0;
		++frameCount;

		// Periodically log output to console for debugging
		if (frameCount % 60 == 0)
			cout << "Motion data: " << zDelta << ", " << xDelta << endl;


		// Check events
		glfwPollEvents();
	}

	// Clean up
	dataLogger.StopLogging();
	rewardDelivery.StopReward();
	motionReader.StopReading();

	core.Shutdown();
	system("pause");
	return 0;
}