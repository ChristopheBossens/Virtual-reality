#include "ResourceManager.h"

std::map<std::string, GLuint> ResourceManager::shaders;
std::map<std::string, GLuint> ResourceManager::textures;

// Resources for shader loading
GLuint ResourceManager::LoadShader(const char *vertexShaderFile, const char *fragmentShaderFile)
{
	std::string vertexShaderCode = ResourceManager::ReadShaderFile(vertexShaderFile);
	std::string fragmentShaderCode = ResourceManager::ReadShaderFile(fragmentShaderFile);

	GLuint vertexShader = ResourceManager::CompileShader(GL_VERTEX_SHADER, vertexShaderCode);
	GLuint fragmentShader = ResourceManager::CompileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

	GLuint shaderProgram = ResourceManager::LinkProgram(vertexShader, fragmentShader);
	return shaderProgram;
}
std::string ResourceManager::ReadShaderFile(const char *shaderFile)
{
	std::string shaderCode;
	std::ifstream fileStream;
	try
	{
		fileStream.open(shaderFile, std::ios::in);
		fileStream.seekg(0, std::ios::end);
		shaderCode.resize((unsigned int)fileStream.tellg());
		fileStream.seekg(0, std::ios::beg);
		fileStream.read(&shaderCode[0], shaderCode.size());
		fileStream.close();
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::ResourceManager::ReadShaderFile: Failed to read shader file" << std::endl;
		std::cout << shaderFile << std::endl;

		if (fileStream.is_open())
			fileStream.close();
	}
	return shaderCode;
}
GLuint ResourceManager::CompileShader(GLenum shaderType, std::string& shaderCode)
{
	GLuint shaderID = glCreateShader(shaderType);

	const char* shaderCodePtr = shaderCode.c_str();
	const int shaderCodeSize = shaderCode.size();

	glShaderSource(shaderID, 1, &shaderCodePtr, &shaderCodeSize);
	glCompileShader(shaderID);

	GLint compileResult;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);

	if (compileResult == 0)
	{
		int logLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(shaderID, logLength, NULL, infoLog);
		
		std::cout << "ERROR::ResourceManager::CompileShader" << std::endl;
		std::cout << infoLog;
		delete[] infoLog;
	}

	return shaderID;
}
GLuint ResourceManager::LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint programID = glCreateProgram();

	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == 0)
	{
		GLint logLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		GLchar *infoLog = new GLchar[logLength];
		glGetProgramInfoLog(programID, logLength, NULL, infoLog);
		std::cout << "ERROR::ResourceManager::LinkProgram:" << std::endl;
		std::cout << infoLog << std::endl;
		delete[] infoLog;
	}

	return programID;
}

// Resources for texture loading
GLuint ResourceManager::LoadTexture(const char* textureFile)
{
	int width, height, channels;
	unsigned char* imageData = SOIL_load_image(textureFile, &width, &height, &channels, SOIL_LOAD_RGB);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(imageData);

	return textureID;
}

GLuint ResourceManager::LoadBlankTexture()
{
	GLubyte textureData = 255;

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 1, 1, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &textureData);

	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

GLuint ResourceManager::LoadNoiseTexure(GLuint width, GLuint height, GLuint maxLuminance, GLuint smoothingKernelSize)
{
	GLubyte* textureData = ResourceManager::GenerateNoiseTexture(width, height, maxLuminance, smoothingKernelSize);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, textureData);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] textureData;
	return textureID;
}

GLuint ResourceManager::LoadGratingTexture(GLuint width, GLuint height, GLuint amplitude, double cyclesPerPixel, double orientation)
{
	GLubyte* textureData = ResourceManager::GenerateGratingTexture(width, height, amplitude, cyclesPerPixel, orientation);

	GLuint textureID;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, textureData);

	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] textureData;
	return textureID;
}
GLubyte* ResourceManager::GenerateNoiseTexture(GLuint width, GLuint height, GLuint maxLuminance, GLuint smoothingKernelSize)
{
	if (smoothingKernelSize % 2 == 0)
		++smoothingKernelSize;

	GLubyte* generatorImage = new GLubyte[(height + smoothingKernelSize)*(width + smoothingKernelSize)];
	GLubyte* noiseImage = new GLubyte[height*width];

	for (int i = 0; i < ((height + smoothingKernelSize)*(width + smoothingKernelSize)); ++i)
		generatorImage[i] = rand() % maxLuminance;

	int smoothOffset = (smoothingKernelSize - 1) / 2;
	int idx;
	for (int i = smoothOffset; i < (height + smoothOffset); ++i)
	{
		for (int j = smoothOffset; j < (width + smoothOffset); ++j)
		{
			GLubyte totalNoiseValue = 0;

			for (int x = -smoothOffset; x <= smoothOffset; ++x)
			{
				for (int y = -smoothOffset; y <= smoothOffset; ++y)
				{
					idx = (i + x)*(width + smoothOffset) + (j + y);
					totalNoiseValue += (generatorImage[idx] / (smoothingKernelSize*smoothingKernelSize));
				}
			}
			idx = (i - smoothOffset)*(width)+(j - smoothOffset);
			noiseImage[idx] = totalNoiseValue;
		}
	}

	delete[] generatorImage;
	return noiseImage;
}
GLubyte* ResourceManager::GenerateGratingTexture(GLuint width, GLuint height, GLuint amplitude, double cyclesPerPixel, double orientation)
{
	GLubyte *gratingData = new GLubyte[width*height];

	int idx;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			double xRot = j*cos(orientation) + i*sin(orientation);
			idx = (i*width) + j;
			gratingData[idx] = 127 + (GLubyte)(amplitude*cos(2.0*3.1458*cyclesPerPixel*xRot));
		}
	}

	return gratingData;
}