#pragma once
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <random>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "ResourceManager.h"
#include "StimulusQuad.h"
#include "ExperimentLoader.h"

class ExperimentManager
{
private:
	GLuint shader2D;
	GLuint gaborShader;
	GLuint apertureShader;
	StimulusQuad stim;

	bool signalTrialOnset;
	bool experimentRunning;
	bool enableMotionReader;
	bool useDAQ;

	glm::mat4 orthoMatrix;
	glm::vec4 gratingParameters;

	ExperimentLoader experimentLoader;

	vector<TrialListElement>::iterator trialIterator;
	vector<TrialStimulus>::iterator stimulusIterator;

	// Random engine, might be needed for randomizing trial data
	mt19937 randomEngine;


	// Sandbox parameters
	bool isAlternating = false;
	bool isDrifting = false;
	bool isVisible = true;
	float alternationTime = 60.0f;
	float driftVelocity = 0.2f;
	vector<float> gratingOrientations;
	int nOrientations;
	int currentOrientationIndex;

public:
	// Experiment state variables
	GLboolean keyStates[256];
	int scrollDirection = 0;
	int mouseX = 0;
	int mouseY = 0;

	ExperimentManager();
	void SetOrthographicMatrix(float left, float width, float height, float bottom);
	void Initialize();
	void LoadExperimentXML(std::string filename);
	
	void InitiateExperiment();
	void UpdateExperiment(unsigned int &frameCount);

	void InitiateSandbox();
	void UpdateSandbox(unsigned int& frameCount);
	void DrawSandbox();

	void ProcessInput();

	bool IsRunning() { return experimentRunning; }
};