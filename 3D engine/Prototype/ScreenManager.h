#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
class ScreenManager
{
protected:
	GLuint frameBufferObject;
	GLuint frameBufferTexture;
	GLuint renderBufferObject;

	GLuint leftQuadVBO;
	GLuint rightQuadVBO;

	int bufferWidth;
	int bufferHeight;

private:
	
	void GenerateOffscreenBuffer();
	void GenerateWindowQuads();

public:
	ScreenManager();
	~ScreenManager();
	void Initialize(int displayWidth, int displayHeight);
	void DrawToTexture();
	void DrawLeftTexture();
	void DrawRightTexture();
};