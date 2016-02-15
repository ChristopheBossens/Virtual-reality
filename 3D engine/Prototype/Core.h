#pragma once
#ifndef INIT_H
#define INIT_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <Windows.h>

namespace Engine
{
	// Contains window info for a single screen
	// Contains width and height, but native resolution will be used if we go fullscreen
	struct WindowInfo 
	{
		std::string name = "default";
		bool useFullscreen = false;
		int width=800;
		int height=600;
		int top=0;
		int left=0;
		bool resizable = true;
	};

	struct ContextInfo
	{
		int majorVersion = 3;
		int minorVersion = 3;
		bool coreProfile = true;
	};

	class Core
	{
	private:
		int nMonitors;
		GLFWmonitor** monitors;
		ContextInfo contextInfo;

		void InitWindow();
		void InitExtensions();

	public:
		Core();
		~Core();

		WindowInfo windowInfo;
		WindowInfo baseWindowInfo;
		WindowInfo secondWindowInfo;

		GLFWwindow* baseWindow;
		GLFWwindow* secondWindow;

		void Initialize(WindowInfo &windowInfo);
		void PrintGLInfo();
		void PrintMonitorInfo();
	};
}

#endif