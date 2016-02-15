#include "Shader_Loader.h"
#include "ScreenManager.h"
#include <iostream>

ScreenManager::ScreenManager()
{
	
}
ScreenManager::~ScreenManager()
{
	glDeleteFramebuffers(1, &frameBufferObject);
	glDeleteRenderbuffers(1, &renderBufferObject);
	glDeleteTextures(1, &frameBufferTexture);
}

void ScreenManager::Initialize(int width, int height, Engine::Core* core)
{
	this->core = core;
	this->bufferWidth = 2 * width;
	this->bufferHeight = height;
	this->GenerateOffscreenBuffer();
	this->GenerateWindowQuads();

	simpleShader = Engine::ShaderLoader::CreateProgram("Shaders\\simple_vertex_shader.glsl", "Shaders\\simple_fragment_shader.glsl");
}

void ScreenManager::DrawToScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glfwMakeContextCurrent(core->baseWindow);
	glUseProgram(simpleShader);
	glViewport(0, 0, core->windowInfo.width, core->windowInfo.height);
	DrawLeftTexture();
	glfwSwapBuffers(core->baseWindow);

	glfwMakeContextCurrent(core->secondWindow);
	glUseProgram(simpleShader);
	glViewport(0, 0, core->windowInfo.width, core->windowInfo.height);
	DrawRightTexture();
	glfwSwapBuffers(core->secondWindow);
}
void ScreenManager::DrawLeftTexture()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	glBindBuffer(GL_ARRAY_BUFFER, leftQuadVBO);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void ScreenManager::DrawRightTexture()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	glBindBuffer(GL_ARRAY_BUFFER, rightQuadVBO);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void ScreenManager::DrawToTexture()
{
	glfwMakeContextCurrent(core->baseWindow);
	glViewport(0, 0, bufferWidth, bufferHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
}

void ScreenManager::GenerateWindowQuads()
{
	GLfloat leftQuadVertices[] = {
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  0.5f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  0.5f, 0.0f,
		1.0f,  1.0f,  0.5f, 1.0f
	};
	GLfloat rightQuadVertices[] = {
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.5f, 1.0f,
		-1.0f, -1.0f,  0.5f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.5f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	glGenBuffers(1, &leftQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, leftQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftQuadVertices), leftQuadVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &rightQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rightQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightQuadVertices), rightQuadVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void ScreenManager::GenerateOffscreenBuffer()
{
	glGenTextures(1, &frameBufferTexture);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferWidth, bufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &renderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer completed" << std::endl;
}