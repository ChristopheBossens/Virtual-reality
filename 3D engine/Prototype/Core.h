#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <vector>
#include <map>

struct WindowParams
{
	int width;
	int height;
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
	int nWindowsCreated;
	bool shouldClose;

	GLuint framebufferWidth, framebufferHeight;
	GLuint framebufferObject, framebufferTexture, renderbufferObject;
	GLuint simpleShader;

	GLFWmonitor** monitors;
	ContextInfo contextInfo;
	std::vector<GLFWwindow*> windowPointers;
	std::vector<WindowParams> windowParams;
	std::vector<GLuint> windowQuads;
	std::map<GLFWwindow*, int> windowOffset;

	void CreateWindows(GLuint width, GLuint height, int nWindows, bool fullscreen);
	void SetWindowHints();
	void InitExtensions();

	void GenerateFramebuffer(GLuint bufferWidth, GLuint bufferHeight);
	void GenerateWindowQuads();
	void RenderWindowQuad(GLuint quadVBO);
public:
	Core();
	~Core();

	void Initialize(GLuint width, GLuint height, int nWindows, bool fullscreen);
	void SetKeyboardCallback(GLFWkeyfun gflwKeyFun);
	void SetMouseMoveCallback(GLFWcursorposfun glfwCursorMoveFun);
	void SetMouseScrollCallback(GLFWscrollfun glfwScrollFun);
	void SetMouseButtonCallback(GLFWmousebuttonfun glfwMouseButtonFun);
	void SetInputMode(int mode, int value);
	void SetShouldClose();

	inline bool ShouldClose() { return shouldClose; }
	inline GLint GetBufferWidth() { return framebufferWidth; }
	inline GLint GetBufferHeight() { return framebufferHeight; }

	int GetWindowOffset(GLFWwindow* window);
	void RenderFramebuffer();
	void SaveScreenshot();
	void Shutdown();

	void PrintGLInfo();
	void PrintMonitorInfo();	
};