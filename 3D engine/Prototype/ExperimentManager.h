#pragma once
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader_Loader.h"
#include "TextureLoader.h"
#include "StimulusQuad.h"

class ExperimentManager
{
private:
	GLuint shader2D;
	GLuint gaborShader;
	GLuint apertureShader;
	StimulusQuad stim;

	bool enableMotionReader;
	bool useDAQ;

	glm::mat4 orthoMatrix;
	glm::vec4 gratingParameters;
	int frameCount;
public:
	void SetOrthographicMatrix(float left, float width, float height, float bottom);
	void LoadExperimentXML(std::string filename);
	void LoadFrameData(std::string filename);
	void LoadTestExperiment(Engine::TextureLoader* textureLoader);

	void RunExperiment();
	void Update(int mouseX, int mouseY);
	void DrawExperiment();
};