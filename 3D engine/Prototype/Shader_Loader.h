#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>

namespace Engine
{
	class ShaderLoader
	{
	private:
		static std::string ReadShader(char* filename);
		static GLuint CreateShader(GLenum shaderType, std::string source, char* shaderName);

	public:
		static GLuint CreateProgram(char* vsFilename, char* fsFilename);
	};
}