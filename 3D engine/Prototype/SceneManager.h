#pragma once
#include <vector>
#include "Square.h"
#include "Camera.h"
#include "TextureLoader.h"

class SceneManager
{
private:
	GLuint defaultShaderProgram;
	
	GLuint containerTexture;
	GLuint blankTexture;
	GLuint noiseTexture;
	GLuint gratingTexture;

	GLuint modelLocation;
	GLuint viewLocation;
	GLuint projectionLocation;
	GLuint textureOffsetLocation;


	float corridorDepth = 100.0f;
	float corridorWidth = 3.0f;
	float wallHeight = 2.0f;

	float rewardZoneDepth = 2.0f;
	float rewardZonePosition1 = 70.0f;
	float rewardZonePosition2 = 50.0f;
	bool inRewardZone = false;

	float wallDistanceMargin = 0.3f;
	float resetPoint = 60.0f;
	float textureOffset;
	std::vector<Shapes::Square*> squareList;

	Camera camera;

	glm::mat4 viewMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 projectionMatrix;

public:
	SceneManager();
	~SceneManager();

	void SetProjectionMatrix(float fieldOfview, float aspectRatio);
	void LoadMesh();
	void LoadTextures(Engine::TextureLoader* textureLoader);
	void LoadShaders();

	void UpdatePosition(float deltaX = 0.0f, float deltaY = 0.0f, float deltaZ = 0.0f);
	void Draw();
};