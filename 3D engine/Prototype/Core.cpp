#include <iostream>
#include "Core.h"
#include "ResourceManager.h"

using namespace std;

Core::Core()
{
	int initResult = glfwInit();

	if (initResult == GL_FALSE)
	{

	}
	shouldClose = false;
}

Core::~Core()
{
	glfwTerminate();
}

// Creates the specified number of of windows. Height represents the height of each window.
// Width represents the sum of the widths of each single window. E.g. if nWindows = 2 and width
// equals 400, two windows with each having a width of 200 will be created
void Core::Initialize(GLuint width, GLuint height, int nWindows, bool fullscreen)
{
	monitors = glfwGetMonitors(&nMonitors);
	if (fullscreen)
	{
		if (nMonitors < nWindows)
		{
			cout << "Core::Initialize::Error: Not enough monitors available for fullscreen mode" << endl;
			return;
		}
	}
	if (width%nWindows != 0)
	{
		cout << "Core::Initialize::Error: Width should be evenly divisible by number of windows" << endl;
		return;
	}
	// Create the appropriate window structures
	windowPointers.resize((size_t)nWindows);
	windowParams.resize((size_t)nWindows);
	windowQuads.resize((size_t)nWindows);

	SetWindowHints();
	
	CreateWindows(width/nWindows, height, nWindows, fullscreen);
	InitExtensions();
	
	// First calculate the framebuffer size based on the windows that were created
	framebufferHeight = 0;
	framebufferWidth = 0;
	for (size_t windowIndex = 0; windowIndex < windowParams.size(); ++windowIndex)
	{
		framebufferHeight = windowParams[windowIndex].height;
		framebufferWidth += windowParams[windowIndex].width;
	}

	// Prepare offscreen buffers and window quads for rendering
	GenerateFramebuffer(framebufferWidth,framebufferHeight);
	GenerateWindowQuads();
	simpleShader = ResourceManager::LoadShader("Shaders\\simple_vertex_shader.glsl", "Shaders\\simple_fragment_shader.glsl");
}
void Core::Shutdown()
{
	glfwTerminate();
}

// Creates nWindows different windows, where each window has the specified width and height
void Core::SetWindowHints()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Core::contextInfo.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Core::contextInfo.minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);
}

void Core::CreateWindows(GLuint width, GLuint height,int nWindows, bool fullscreen)
{
	GLFWwindow* sharedContextWindow = nullptr;
	GLFWmonitor* currentMonitor = nullptr;

	nWindowsCreated = 0;
	windowOffset.clear();

	int currentWindowOffset = 0;
	for (int windowIdx = 0; windowIdx < nWindows; ++windowIdx)
	{
		if (fullscreen)
			currentMonitor = *(monitors + windowIdx);
		if (windowIdx > 0)
			sharedContextWindow = windowPointers[0];

		windowPointers[windowIdx] = glfwCreateWindow(width, height, "window", currentMonitor, sharedContextWindow);

		if (windowPointers[windowIdx] == NULL)
		{
			cout << "Core::CreateWindows::Error: Could not create the requested number of windows";
			return;
		}

		glfwGetWindowSize(windowPointers[windowIdx], &windowParams[windowIdx].width, &windowParams[windowIdx].height);
		
		++nWindowsCreated;
		windowOffset[windowPointers[windowIdx]] = currentWindowOffset;
		currentWindowOffset += windowParams[windowIdx].width;
	}
	glfwMakeContextCurrent(windowPointers[0]);
	glfwSwapInterval(1);
	shouldClose = false;

	cout << "Core-InitWindow: Windows succesfully created" << endl;
}
void Core::InitExtensions()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "Core-InitExtensions: Could not load glew extensions" << endl;
		glfwTerminate();
		return;
	}
	cout << "Core-InitExtensions: Extensions loaded." << endl;
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

// Window callback functions
void Core::SetKeyboardCallback(GLFWkeyfun gflwKeyFun)
{
	for (size_t windowIndex = 0; windowIndex < windowPointers.size(); ++windowIndex)
		glfwSetKeyCallback(windowPointers[windowIndex], gflwKeyFun);
}
void Core::SetMouseMoveCallback(GLFWcursorposfun glfwCursorMoveFun)
{
	for (size_t windowIndex = 0; windowIndex < windowPointers.size(); ++windowIndex)
		glfwSetCursorPosCallback(windowPointers[windowIndex], glfwCursorMoveFun);
}
void Core::SetMouseScrollCallback(GLFWscrollfun glfwScrollFun)
{
	for (size_t windowIndex = 0; windowIndex < windowPointers.size(); ++windowIndex)
		glfwSetScrollCallback(windowPointers[windowIndex], glfwScrollFun);
}
void Core::SetMouseButtonCallback(GLFWmousebuttonfun glfwMouseButtonFun)
{
	for (size_t windowIndex = 0; windowIndex < windowPointers.size(); ++windowIndex)
		glfwSetMouseButtonCallback(windowPointers[windowIndex], glfwMouseButtonFun);
}
void Core::SetInputMode(int mode, int value)
{
	for (size_t windowIndex = 0; windowIndex < windowPointers.size(); ++windowIndex)
		glfwSetInputMode(windowPointers[windowIndex], GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void Core::SetShouldClose()
{
	shouldClose = true;
}


int Core::GetWindowOffset(GLFWwindow* window)
{
	return windowOffset[window];
}

// Draw offscreen framebuffer to different windows
void Core::SaveScreenshot()
{
	/*
	unsigned char* framebufferData = new unsigned char[bufferWidth*bufferHeight * 3];

	glReadPixels(0, 0, bufferWidth, bufferHeight, GL_RGB, GL_UNSIGNED_BYTE, (void*)framebufferData);
	int result = SOIL_save_image("C:\\Users\\Christophe\\Documents\\GitHub\\Virtual-reality\\3D engine\\Debug\\Images\\screenshot1.bmp", SOIL_SAVE_TYPE_BMP, bufferWidth, bufferHeight, 3, framebufferData);
	if (result == 0)
		std::cout << SOIL_last_result() << std::endl;
	else
		std::cout << "Screenshot saved" << std::endl;

	delete[] framebufferData;
	*/
}
void Core::RenderFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	for (int windowIndex = 0; windowIndex < nWindowsCreated; ++windowIndex)
	{
		glfwMakeContextCurrent(windowPointers[windowIndex]);
		glUseProgram(simpleShader);
		glViewport(0, 0, windowParams[windowIndex].width, windowParams[windowIndex].height);
		RenderWindowQuad(windowQuads[windowIndex]);
		glfwSwapBuffers(windowPointers[windowIndex]);
	}

	// Make offscreen framebuffer default target for drawing commands again
	glfwMakeContextCurrent(windowPointers[0]);
	glViewport(0, 0, framebufferWidth, framebufferHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);
}

void Core::GenerateFramebuffer(GLuint bufferWidth, GLuint bufferHeight)
{
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferWidth, bufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &renderbufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &framebufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferObject);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Offscreen framebuffer size:" << bufferWidth << "*" << bufferHeight << std::endl;
}

void Core::GenerateWindowQuads()
{
	GLfloat quadVertices[] = {
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  0.5f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  0.5f, 0.0f,
		1.0f,  1.0f,  0.5f, 1.0f
	};
	GLfloat textureWidth = 1.0f / (GLfloat)nWindowsCreated;

	for (int windowIndex = 0; windowIndex < nWindowsCreated; ++windowIndex)
	{
		quadVertices[2] = windowIndex*textureWidth;
		quadVertices[6] = windowIndex*textureWidth;
		quadVertices[14] = windowIndex*textureWidth;

		quadVertices[10] = (windowIndex + 1)*textureWidth;
		quadVertices[18] = (windowIndex + 1)*textureWidth;
		quadVertices[22] = (windowIndex + 1)*textureWidth;

		glGenBuffers(1, &windowQuads[windowIndex]);
		glBindBuffer(GL_ARRAY_BUFFER, windowQuads[windowIndex]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Core::RenderWindowQuad(GLuint quadVBO)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}