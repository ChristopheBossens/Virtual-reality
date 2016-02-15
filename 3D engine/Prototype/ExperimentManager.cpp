#include "ExperimentManager.h"


using namespace Engine;
void ExperimentManager::LoadExperimentXML(std::string filename)
{
}

void ExperimentManager::RunExperiment()
{
}

void ExperimentManager::SetOrthographicMatrix(float left, float width, float height, float bottom)
{
	orthoMatrix = glm::ortho(left, width, height, bottom);
}
void ExperimentManager::LoadTestExperiment(TextureLoader* textureLoader)
{
	shader2D = ShaderLoader::CreateProgram("Shaders\\vertex_shader_2D.glsl", "Shaders\\fragment_shader_2D.glsl");
	gaborShader = ShaderLoader::CreateProgram("Shaders\\gabor_vertex_shader.glsl", "Shaders\\gabor_fragment_shader.glsl");
	apertureShader = ShaderLoader::CreateProgram("Shaders\\aperture_vertex_shader.glsl", "Shaders\\aperture_fragment_shader.glsl");

	stim.SetPosition(100, 100);
	stim.SetSize(300, 300);
	stim.InitRenderData();

	gratingParameters = glm::vec4(0.5f, 100.0f, 0.05f, 0.0f);

	frameCount = 0;
}

void ExperimentManager::Update(int mouseX, int mouseY)
{
	stim.SetPosition(mouseX, mouseY);
	stim.InitRenderData();
}

void ExperimentManager::DrawExperiment()
{
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(apertureShader);
	glUniformMatrix4fv(glGetUniformLocation(apertureShader, "projection"), 1, GL_FALSE, glm::value_ptr(orthoMatrix));
	glUniform4fv(glGetUniformLocation(apertureShader, "gratingParameters"), 1, glm::value_ptr(gratingParameters));
	stim.Draw();
}