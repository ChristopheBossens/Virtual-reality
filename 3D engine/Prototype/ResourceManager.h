#pragma once

#include <map>
#include <string>
#include <fstream>
#include <iostream>

#include <GL/glew.h>

#include <SOIL.h>

class ResourceManager
{
public:
	static std::map<std::string, GLuint> shaders;
	static std::map<std::string, GLuint> textures;

	static GLuint LoadShader(const char *vertexShaderFile, const char *fragmentShaderFile);
	
	static GLuint LoadTexture(const char *texureFile);
	static GLuint LoadBlankTexture();
	static GLuint LoadNoiseTexure(GLuint width, GLuint height, GLuint maxLuminance, GLuint smoothingKernelSize);
	static GLuint LoadGratingTexture(GLuint width, GLuint height, GLuint amplitude, double cyclesPerPixel, double orientation);

private:
	ResourceManager() {}
	static std::string ReadShaderFile(const char *shaderFile);
	static GLuint CompileShader(GLenum shaderType,std::string &shaderCode);
	static GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

	static GLubyte* GenerateNoiseTexture(GLuint width, GLuint height, GLuint maxLuminance, GLuint smoothingKernelSize);
	static GLubyte* GenerateGratingTexture(GLuint width, GLuint height, GLuint amplitude, double cyclesPerPixel, double orientation);
};