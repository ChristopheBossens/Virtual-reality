#pragma once
#include <vector>
#include <map>
#include <string>
#include "Square.h"
#include "Camera.h"
#include "ResourceManager.h"

class TextureCorridor
{
private:
	GLuint defaultShaderProgram;

	Camera camera;

	glm::mat4 viewMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 projectionMatrix;

	GLuint modelLocation;
	GLuint viewLocation;
	GLuint projectionLocation;

	float corridorWidth = 3.0f;
	float corridorDepth = 100.0f;
	float wallHeight = 9.0f;

	std::map<std::string, GLuint> textureMap;
	std::vector<Shapes::Square*> squareList;

	int frameCount = 0;
	int nTextures = 30;
public:
	TextureCorridor();

	void LoadMesh(int corridorType);
	void LoadTextures();
	void LoadShaders();

	void UpdatePosition(float deltaX, float deltaY, float deltaZ);
	void Draw();
};