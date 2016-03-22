#include "TexturedCorridor.h"
#include "ResourceManager.h"
using namespace Shapes;
using namespace std;
TextureCorridor::TextureCorridor()
{
	camera.SetY(1.5f);
}

void LoadScene(int sceneIndex)
{
}

// Load functions are performed on initialization
void TextureCorridor::LoadMesh(int corridorType)
{
	for (int i = 0; i < squareList.size(); ++i)
		delete squareList[i];
	squareList.clear();

	Square* square = new Square;

	vector<float> xValues = { corridorWidth,corridorWidth, 2.0f*corridorWidth, 2.0f*corridorWidth, corridorWidth };

	
	square->SetScaling(2.0f*corridorWidth, 60*wallHeight);
	square->SetRotation(-90.0f, 0.0f, 0.0f);
	square->SetPosition(0.0f, 0.0f, -(corridorDepth-wallHeight));
	square->SetTextureID(textureMap["floor"]);
	squareList.push_back(square);

	switch (corridorType)
	{
		// Straight corridor
		case 1:
			for (size_t wallIndex = 1; wallIndex <= 60; ++wallIndex)
			{
				float x1 = corridorWidth;
				float x2 = corridorWidth;
				float y1 = wallHeight ;
				float y2 = 0.0f;
				float z1 = -((wallIndex - 1)*wallHeight);
				float z2 = z1 - wallHeight;

				string texname = "tex" + to_string(((wallIndex - 1) % nTextures) + 1);

				// left wall #1
				square = new Square;
				square->SetTopLeft(x1, y1, z1);;
				square->SetBottomLeft(x1, y2, z1);
				square->SetTopRight(x2, y1, z2);
				square->SetBottomRight(x2, y2, z2);
				square->SetTextureID(textureMap[texname]);
				square->Apply();
				squareList.push_back(square);

				// right wall
				square = new Square;
				square->SetTopLeft(-x1, y1, z1);;
				square->SetBottomLeft(-x1, y2, z1);
				square->SetTopRight(-x2, y1, z2);
				square->SetBottomRight(-x2, y2, z2);
				square->SetTextureID(textureMap[texname]);
				square->Apply();
				squareList.push_back(square);
			}
			break;

		// Corridor with varying geometry
		case 2:
			for (size_t wallIndex = 1; wallIndex <= 60; ++wallIndex)
			{
				float x1 = xValues[(wallIndex - 1) % xValues.size()];
				float x2 = xValues[(wallIndex) % xValues.size()];
				float y1 = wallHeight;
				float y2 = 0.0f;
				float z1 = -((wallIndex - 1)*wallHeight);
				float z2 = z1 - wallHeight;

				string texname = "tex" + to_string(((wallIndex - 1) % nTextures) + 1);

				// left wall #1
				square = new Square;
				square->SetTopLeft(x1, y1, z1);;
				square->SetBottomLeft(x1, y2, z1);
				square->SetTopRight(x2, y1, z2);
				square->SetBottomRight(x2, y2, z2);
				square->SetTextureID(textureMap[texname]);
				square->Apply();
				squareList.push_back(square);

				// right wall
				square = new Square;
				square->SetTopLeft(-x1, y1, z1);;
				square->SetBottomLeft(-x1, y2, z1);
				square->SetTopRight(-x2, y1, z2);
				square->SetBottomRight(-x2, y2, z2);
				square->SetTextureID(textureMap[texname]);
				square->Apply();
				squareList.push_back(square);
			}
			break;
	default:
		break;
	}
}

void TextureCorridor::LoadShaders()
{
	defaultShaderProgram = ResourceManager::LoadShader("Shaders\\vertex_shader.glsl", "Shaders\\fragment_shader.glsl");

	modelLocation = glGetUniformLocation(defaultShaderProgram, "model");
	viewLocation = glGetUniformLocation(defaultShaderProgram, "view");
	projectionLocation = glGetUniformLocation(defaultShaderProgram, "projection");
}
void TextureCorridor::LoadTextures()
{
	textureMap["floor"] = ResourceManager::LoadNoiseTexure(512, 512, 128,7);

	for (int imageIndex = 1; imageIndex <= nTextures; ++imageIndex)
	{
		//string filename = "Images\\" + to_string(imageIndex) + ".bmp";
		string filename = "Images\\" + to_string(18) + ".bmp";
		string mapname = "tex" + to_string(imageIndex);
		textureMap[mapname] = ResourceManager::LoadTexture(filename.c_str());
	}
}


// Update function receives motion sensor delta values

void TextureCorridor::UpdatePosition(float deltaX, float deltaY, float deltaZ)
{
	float newZ = camera.GetZ() + 20.0f*deltaZ;
	
	//if (newZ <= -10.0f*2.0f*wallHeight)
	//	newZ = 0.0f;

	if (newZ <= -30.0f*wallHeight)
		newZ = 0.0f;

	camera.SetZ(newZ);

	if (frameCount % 60 == 0)
	{
		cout << newZ << endl;
		frameCount = 0;
	}
	++frameCount;
}


void TextureCorridor::Draw()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(defaultShaderProgram);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, camera.pViewMatrix);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, camera.pProjectionMatrix);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	
	for (size_t squareIndex = 0; squareIndex < squareList.size(); ++squareIndex)
		squareList[squareIndex]->Draw();
}