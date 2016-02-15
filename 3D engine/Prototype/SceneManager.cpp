#include "SceneManager.h"
#include "Shader_Loader.h"
#include "TextureLoader.h"

using namespace std;
using namespace Shapes;
using namespace Engine;

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::SetProjectionMatrix(float fieldOfView, float aspectRatio)
{
	this->projectionMatrix = glm::perspective(fieldOfView, aspectRatio, 0.1f, 100.0f);
	modelMatrix = glm::mat4();
}
void SceneManager::LoadMesh()
{
	Square* square = new Square;
	squareList.clear();
	// back wall
	square->SetColor(0.5f, 0.5f, 0.5f);
	square->SetScaling(corridorWidth, wallHeight);
	square->SetPosition(0.0f, wallHeight / 2.0f, -corridorDepth);
	square->SetTextureID(containerTexture);
	squareList.push_back(square);

	// Front wall
	square = new Square;
	square->SetColor(0.5f, 0.5f, 0.9f);
	square->SetScaling(corridorWidth, wallHeight);
	square->SetPosition(0.0f, wallHeight / 2.0f, corridorDepth);
	square->SetTextureID(containerTexture);
	squareList.push_back(square);
	
	// Left wall
	square = new Square;
	square->SetColor(1.0f, 1.0f, 1.0f);
	square->SetScaling(corridorDepth, wallHeight);
	square->SetRotation(0.0f, 90.0f, 0.0f);
	square->SetPosition(-corridorWidth, wallHeight / 2.0f, 0.0f);
	square->SetTextureID(gratingTexture);
	squareList.push_back(square);

	// Right wall
	square = new Square;
	square->SetColor(1.0f, 1.0f, 1.0f);
	square->SetScaling(corridorDepth, wallHeight);
	square->SetRotation(0.0f, 90.0f, 0.0f);
	square->SetPosition(corridorWidth, wallHeight / 2.0, 0.0f);
	square->SetTextureID(gratingTexture);
	squareList.push_back(square);

	// Floor
	square = new Square;
	square->SetColor(1.0f, 1.0f, 1.0f);
	square->SetScaling(corridorWidth, corridorDepth);
	square->MatchTextureToScale();
	square->SetRotation(-90.0f, 0.0f, 0.0f);
	square->SetPosition(0.0f, -wallHeight / 2.0f, 0.0f);
	square->SetTextureID(noiseTexture);
	squareList.push_back(square);

	// First reward zone
	square = new Square;
	square->SetColor(0.2f, 0.9f, 0.2f);
	square->SetScaling(corridorWidth, rewardZoneDepth);
	square->SetRotation(-90.0f, 0.0f, 0.0f);
	square->SetPosition(0.0f, -wallHeight / 2.0f + 0.001f, rewardZonePosition1);
	square->SetTextureID(blankTexture);
	squareList.push_back(square);

	// Second reward zone
	square = new Square;
	square->SetColor(0.2f, 0.9f, 0.2f);
	square->SetScaling(corridorWidth, rewardZoneDepth);
	square->SetRotation(-90.0f, 0.0f, 0.0f);
	square->SetPosition(0.0f, -wallHeight / 2.0f + 0.001f, rewardZonePosition2);
	square->SetTextureID(blankTexture);
	squareList.push_back(square);
}
void SceneManager::LoadShaders()
{
	defaultShaderProgram = ShaderLoader::CreateProgram("Shaders\\vertex_shader.glsl", "Shaders\\fragment_shader.glsl");

	modelLocation = glGetUniformLocation(defaultShaderProgram, "model");
	viewLocation = glGetUniformLocation(defaultShaderProgram, "view");
	projectionLocation = glGetUniformLocation(defaultShaderProgram, "projection");

	textureOffsetLocation = glGetUniformLocation(defaultShaderProgram, "textureOffset");
}

void SceneManager::LoadTextures(TextureLoader* textureLoader)
{
	containerTexture = textureLoader->LoadRGB("Images\\container.jpg");
	blankTexture = textureLoader->LoadBlank();
	noiseTexture = textureLoader->LoadNoiseTexture(512, 512, 128, 7);
	gratingTexture = textureLoader->LoadGratingTexture(512, 512, 100, 0.02, 1.7);
}
void SceneManager::UpdatePosition(float deltaX, float deltaY, float deltaZ)
{
	float newX = camera.GetX() + deltaX;
	float newY = camera.GetY() + deltaY;
	float newZ = camera.GetZ() + deltaZ;

	// Check if updated position fals outside the corridor
	if (!((newX - wallDistanceMargin < -corridorWidth) | ((newX + wallDistanceMargin) > corridorWidth)))
		camera.SetX(newX);
	if (!((newZ - wallDistanceMargin) < -corridorDepth) | ((newZ + wallDistanceMargin) > corridorDepth))
		camera.SetZ(newZ);

	// Check if position needs to be reset to the start
	if (newZ < resetPoint)
	{
		textureOffset += 0.1f;
		newZ += 20.0f;
	}
	
	camera.SetZ(newZ);

	// Update cameraposition

	// Check if we are in the reward zone
	if (!inRewardZone && (newZ < (rewardZonePosition1 + rewardZoneDepth) &&
		(newZ >(rewardZonePosition1 - rewardZoneDepth))))
	{
		inRewardZone = true;
	}
	if (inRewardZone && (newZ > rewardZonePosition1 + rewardZoneDepth ||
		newZ < rewardZonePosition1 - rewardZoneDepth))
		inRewardZone = false;
}

void SceneManager::Draw()
{
	viewMatrix = camera.GetViewMatrix();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(defaultShaderProgram);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform1f(textureOffsetLocation, 0.0f);

	for (int i = 0; i < squareList.size(); ++i)
		squareList[i]->Draw();
}