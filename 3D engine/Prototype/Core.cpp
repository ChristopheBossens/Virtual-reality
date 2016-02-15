#include <iostream>
#include <Windows.h>
#include "Core.h"

using namespace Engine;
using namespace std;

Core::Core()
{
	// Initialize opengl 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Core::contextInfo.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Core::contextInfo.minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);

	// Retrieve monitor info
	monitors = glfwGetMonitors(&nMonitors);
}

Core::~Core()
{}

void Core::Initialize(WindowInfo &windowInfo)
{
	this->windowInfo = windowInfo;

	InitWindow();
	InitExtensions();
}
void Core::InitWindow()
{
	if (this->windowInfo.useFullscreen)
	{
		baseWindow = glfwCreateWindow(windowInfo.width, windowInfo.height, "BaseWindow", *(monitors + 0), nullptr);
		secondWindow = glfwCreateWindow(windowInfo.width, windowInfo.height, "SecondWindow", *(monitors + 1), baseWindow);
	}
	else
	{
		baseWindow = glfwCreateWindow(windowInfo.width, windowInfo.height, "Default", nullptr, nullptr);
		secondWindow = glfwCreateWindow(windowInfo.width, windowInfo.height, "Second", nullptr, baseWindow);
	}
	if (baseWindow == NULL | secondWindow == NULL)
	{
		cout << "Core-InitWindow: could not create window" << endl;
		glfwTerminate();
		return;
	}

	glfwGetWindowSize(baseWindow, &baseWindowInfo.width, &baseWindowInfo.height);
	glfwGetWindowSize(secondWindow, &secondWindowInfo.width, &secondWindowInfo.height);

	glfwMakeContextCurrent(baseWindow);
	glfwSwapInterval(1);
}
void Core::InitExtensions()
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

void Core::PrintGLInfo()
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
void Core::PrintMonitorInfo()
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