#pragma once
#ifndef INIT_H
#define INIT_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <Windows.h>

namespace Core
{
	struct WindowInfo 
	{
		std::string name = "default";
		int width=800;
		int height=600;
		int top=30;
		int left=30;
		bool resizable = true;
	};

	struct ContextInfo
	{
		int majorVersion = 3;
		int minorVersion = 3;
		bool coreProfile = true;
	};

	class InitGlfw
	{
	private:
		int nMonitors;
		GLFWmonitor** monitors;

		bool fullScreenMode = false;
		
		ContextInfo contextInfo;

		void InitWindow();
		void InitContext();
		void InitExtensions();
		void SetWindowInfo();
		void SetMotionReader();

		void GetMonitors();
	public:
		InitGlfw();
		~InitGlfw();

		WindowInfo windowInfo;
		bool twinCameraMode;
		GLFWwindow* window;
		GLFWwindow* window2;

		void Init();
		void PrintGLInfo();
		void PrintMonitorInfo();
		std::string GetApplicationPath();
	};
}

#endif