#include <iostream>
#include "Init.h"

using namespace Core;
using namespace std;

InitGlfw::InitGlfw()
{
	
}

InitGlfw::~InitGlfw()
{}

void InitGlfw::Init()
{
	InitContext();
	GetMonitors();
	GetApplicationPath();
	
	SetWindowInfo();
	InitWindow();
	InitExtensions();
	PrintGLInfo();
}
void InitGlfw::GetMonitors()
{
	cout << "Retrieving monitor information...";
	monitors = glfwGetMonitors(&nMonitors);
	cout << " Detected " << nMonitors << " connected monitor(s)" << endl;
}
void InitGlfw::InitContext()
{
	cout << "Initializing GLFW...";
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, InitGlfw::contextInfo.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, InitGlfw::contextInfo.minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	cout << "DONE" << endl;
}
void InitGlfw::SetWindowInfo()
{
	cout << endl;
	cout << "**************************" << endl;
	cout << "Environment configuration" << endl;
	cout << "**************************" << endl;
	cout << "i\tRetrieves a list of available screen modes." << endl;
	cout << "f\tSet fullscreen mode" << endl;
	cout << "w\tSet windowed mode (default)" << endl;
	cout << "x\tSet resolution width" << endl;
	cout << "y\tSet resolution height" << endl << endl;
	cout << "Press o to continue" << endl;

	char userInput;
	while (true)
	{
		cin >> userInput;
		if (userInput == 'f')
		{
			
			if (nMonitors < 2)
			{
				cout << "Only one monitor detected, fullscreen mode is not available" << endl;
				continue;
			}
			fullScreenMode = true;
			cout << "Fullscreen mode enabled" << endl;
		}
		if (userInput == 'w')
		{
			fullScreenMode = false;
			cout << "Window mode enabled" << endl;
		}
		if (userInput == 'i')
		{
			PrintMonitorInfo();
		}
		if (userInput == 'o')
		{
			cout << "Starting virtual environment" << endl;
			break;
		}
	}
	cout << endl;
}
void InitGlfw::InitWindow()
{
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);

	cout << "Creating first window...";
	if (fullScreenMode)
		window = glfwCreateWindow(windowInfo.width, windowInfo.height, "Default", *(monitors + 0), nullptr);
	else
		window = glfwCreateWindow(windowInfo.width, windowInfo.height, "Default", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, windowInfo.width, windowInfo.height);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	if (window == nullptr)
	{
		cout << "FAILED" << endl;
		glfwTerminate();
		return;
	}
	cout << "DONE" << endl;

	cout << "Creating second window...";
	if (fullScreenMode)
		window2 = glfwCreateWindow(windowInfo.width, windowInfo.height, "Second", *(monitors+1), window);
	else
		window2 = glfwCreateWindow(windowInfo.width, windowInfo.height, "Second", nullptr, window);

	glfwMakeContextCurrent(window2);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, windowInfo.width, windowInfo.height);
	glClearColor(0.6f, 0.5f, 0.7f, 1.0f);
	if (window2 == nullptr)
	{
		cout << "FAILED" << endl;
		glfwTerminate();
		return;
	}
	cout << "DONE" << endl;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
}
void InitGlfw::InitExtensions()
{
	cout << "Start OpenGL extension initialization...";
	glewExperimental = GL_TRUE;
	if (glewInit())
	{
		cout << "FAILED" << endl;
		glfwTerminate();
		return;
	}
	cout << "DONE" << endl;
}

void InitGlfw::PrintGLInfo()
{
	const unsigned char* renderer = glGetString(GL_RENDERER);
	const unsigned char* vendor = glGetString(GL_VENDOR);
	const unsigned char* version = glGetString(GL_VERSION);
	GLint dims[2];
	GLint fbWidth;
	GLint fbHeight;
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &dims[0]);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &fbWidth);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &fbHeight);

	cout << endl;
	cout << "OpenGL Vendor: " << vendor << endl;
	cout << "OpenGL Renderer: " << renderer << endl;
	cout << "OpenGL Version: " << version << endl;
	cout << "Maximum viewport size: " << dims[0] << " by " << dims[1] << endl;
	cout << "Maximum framebuffer size: " << fbWidth << " by " << fbHeight << endl;
	cout << endl;
}
void InitGlfw::PrintMonitorInfo()
{
	for (int monitorIndex = 0; monitorIndex < nMonitors; ++monitorIndex)
	{
		int width, height, nModes;
		glfwGetMonitorPhysicalSize(*(monitors + monitorIndex), &width, &height);
		const GLFWvidmode* vidMode = glfwGetVideoModes(*(monitors + monitorIndex), &nModes);

		cout << endl;
		cout << "Monitor " << monitorIndex << ": " << glfwGetMonitorName(*(monitors + monitorIndex)) << endl;
		cout << "Physical size: " << width << " by " << height << " mm" << endl;

		for (int vmIndex = 0; vmIndex < nModes;++vmIndex)
		{
			cout << vmIndex << ": " << vidMode[vmIndex].width << "x" << vidMode[vmIndex].height << " @ "
				<< vidMode[vmIndex].refreshRate << endl;
		}
	}
}
std::string InitGlfw::GetApplicationPath()
{
	char result[MAX_PATH];
	int pathLength = GetModuleFileNameA(NULL, result, MAX_PATH);
	int lastBackslash = pathLength - 1;
	for (lastBackslash;lastBackslash > 0;--lastBackslash)
	{
		if (result[lastBackslash] == '\\')
			break;
	}
	cout << result << endl;
	return string(result, lastBackslash);
}