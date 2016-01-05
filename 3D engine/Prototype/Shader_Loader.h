#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>

namespace Core
{
	class ShaderLoader
	{
	private:
		char* filename;
		std::string ReadShader(char* filename);
		GLuint CreateShader(GLenum shaderType, std::string source, char* shaderName);

	public:
		ShaderLoader(void);
		~ShaderLoader(void);
		GLuint CreateProgram(char* vsFilename, char* fsFilename);
	};
}