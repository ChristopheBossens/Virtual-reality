#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Core.h"
class ScreenManager
{
protected:
	GLuint frameBufferObject;
	GLuint frameBufferTexture;
	GLuint renderBufferObject;

	GLuint leftQuadVBO;
	GLuint rightQuadVBO;

	GLuint simpleShader;

	int bufferWidth;
	int bufferHeight;

	Engine::Core* core;
private:
	
	void GenerateOffscreenBuffer();
	void GenerateWindowQuads();
	void DrawLeftTexture();
	void DrawRightTexture();

public:
	ScreenManager();
	~ScreenManager();

	void Initialize(int displayWidth, int displayHeight, Engine::Core* core);
	
	void DrawToTexture();
	void DrawToScreen();
};