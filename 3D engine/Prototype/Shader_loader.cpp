#include "Shader_Loader.h"
#include <iostream>
using namespace Core;

ShaderLoader::ShaderLoader(void) {}
ShaderLoader::~ShaderLoader(void) {}

// Returns a string representation of the code in filename
std::string ShaderLoader::ReadShader(char *filename)
{
	this->filename = filename;
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);

	if (!file.good())
	{
		std::cout << "Cannot open shader file: " << filename << std::endl;
		std::cin.get();
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();

	return shaderCode;
}

// Creates a shader of type shaderType with name shadername
GLuint ShaderLoader::CreateShader(GLenum shaderType, std::string source, char* shaderName)
{
	int compileResult = 0;

	GLuint shader = glCreateShader(shaderType);

	const char* shaderCodePtr = source.c_str();
	const int shaderCodeSize = source.size();

	glShaderSource(shader, 1, &shaderCodePtr, &shaderCodeSize);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	if (compileResult == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
		std::cout << "Error compiling shader " <<this->filename << ": " << infoLog << std::endl;
		delete[] infoLog;
		return -1;
	}

	return shader;
}

// Creates a shader program based on available shaders
GLuint ShaderLoader::CreateProgram(char* vertexShaderFilename, char* fragmentShaderFilename)
{
	std::string vertexShaderCode = ReadShader(vertexShaderFilename);
	std::string fragmentShaderCode = ReadShader(fragmentShaderFilename);

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode, "vertexShader");
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode, "fragmentShader");

	if (vertexShader == -1 || fragmentShader == -1)
		return -1;

	int linkResult = 0;
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkResult);

	if (linkResult == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(shaderProgram, infoLogLength, NULL, infoLog);
		std::cout << "Error linking shader program: " << infoLog << std::endl;
		delete[] infoLog;
		return -1;
	}

	return shaderProgram;
}