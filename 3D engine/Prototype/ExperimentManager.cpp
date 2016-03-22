#include "ExperimentManager.h"
#include "ExperimentLoader.h"

ExperimentManager::ExperimentManager()
{
	signalTrialOnset = false;
	experimentRunning = false;
	enableMotionReader = false;
	useDAQ = false;

	randomEngine = default_random_engine{};
}
void ExperimentManager::Initialize()
{
	shader2D = ResourceManager::LoadShader("Shaders\\vertex_shader_2D.glsl", "Shaders\\fragment_shader_2D.glsl");
	gaborShader = ResourceManager::LoadShader("Shaders\\gabor_vertex_shader.glsl", "Shaders\\gabor_fragment_shader.glsl");
	apertureShader = ResourceManager::LoadShader("Shaders\\aperture_vertex_shader.glsl", "Shaders\\aperture_fragment_shader.glsl");
}

// Load an XML file and prepare any textures if necessary
void ExperimentManager::LoadExperimentXML(std::string filename)
{
	// Load data
	experimentLoader.LoadExperiment(filename);

	// Upload textures
	cout << "Checking stimulus list\n";

	std::map<string, StimulusListElement>::iterator it;
	for (it = experimentLoader.stimulusList.begin(); it != experimentLoader.stimulusList.end(); ++it)
	{
		cout << "NAME: " << it->second.stimulusName << endl;
		if (it->second.stimulusType == IMAGE)
		{
			it->second.texID = ResourceManager::LoadTexture(it->second.stimulusLocation.c_str());
			if ((int)it->second.texID == -1)
				cout << "STATUS: FAILED TO UPLOAD" << endl;
			else
				cout << "STATUS: UPLOAD OK" << endl;
		}
		if (it->second.stimulusType == GABOR_GRATING)
		{
			cout << "STATUS: GABOR OK" << endl;
		}
		if (it->second.stimulusType == APERTURE_GRATING)
		{
			cout << "STATUS: APERTURE OK" << endl;
		}
	}
	// 
	cout << "Load completed" << endl;
}
void ExperimentManager::InitiateExperiment()
{
	if (!experimentLoader.trialList.empty())
	{
		trialIterator = experimentLoader.trialList.begin();
		experimentRunning = true;
	}
}
void ExperimentManager::UpdateExperiment(unsigned int& frameCount)
{
	// Show information at the start of  trial
	if (frameCount == 0)
	{
		cout << "Trial id: " << trialIterator->id << endl;
		cout << "Condition: " << trialIterator->condition << endl;

		if (signalTrialOnset)
		{

		}
	}

	glDisable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw all trial stimuli to the screen
	for (stimulusIterator = trialIterator->trialStimuli.begin(); stimulusIterator != trialIterator->trialStimuli.end(); ++stimulusIterator)
	{
		switch (stimulusIterator->stimulusListElement->stimulusType)
		{
		case (IMAGE) :
			if (stimulusIterator->stimParams[4] < frameCount && stimulusIterator->stimParams[5] > frameCount)
			{
				glUseProgram(shader2D);
				glUniformMatrix4fv(glGetUniformLocation(shader2D, "projection"), 1, GL_FALSE, glm::value_ptr(orthoMatrix));
				stim.SetPosition(stimulusIterator->stimParams[0], stimulusIterator->stimParams[1]);
				stim.SetSize(stimulusIterator->stimParams[2], stimulusIterator->stimParams[3]);
				stim.InitRenderData();
				stim.Draw(stimulusIterator->stimulusListElement->texID);
			}
			break;
		case (APERTURE_GRATING) :
			if (stimulusIterator->stimParams[4] < frameCount && stimulusIterator->stimParams[5] > frameCount)
			{
				gratingParameters.x = stimulusIterator->extraParams[0];
				gratingParameters.y = stimulusIterator->extraParams[1];
				gratingParameters.z = stimulusIterator->extraParams[2];
				gratingParameters.w = stimulusIterator->extraParams[3];

				glUseProgram(apertureShader);
				glUniformMatrix4fv(glGetUniformLocation(apertureShader, "projection"), 1, GL_FALSE, glm::value_ptr(orthoMatrix));
				glUniform4fv(glGetUniformLocation(apertureShader, "gratingParameters"), 1, glm::value_ptr(gratingParameters));

				stim.SetPosition(stimulusIterator->stimParams[0], stimulusIterator->stimParams[1]);
				stim.SetSize(stimulusIterator->stimParams[2], stimulusIterator->stimParams[3]);
				stim.InitRenderData();
				stim.Draw();
			}
			break;
		case (GABOR_GRATING) :
			break;
		default:
			break;
		}
	}

	// Check transition event
	if (trialIterator->transitionEvent.event == FRAME_COUNT && frameCount >= trialIterator->transitionEvent.parameters[0])
	{
		cout << "Trial ended" << endl;
		frameCount = -1;
		++trialIterator;
		if (trialIterator == experimentLoader.trialList.end())
			experimentRunning = false;
	}

	// Check reward event
}

void ExperimentManager::SetOrthographicMatrix(float left, float width, float height, float bottom)
{
	orthoMatrix = glm::ortho(left, width, height, bottom);
}

// Sandbox functions
void ExperimentManager::InitiateSandbox()
{
	experimentRunning = true;

	stim.SetSize(100, 100);
	stim.InitRenderData();

	gratingParameters = glm::vec4(0.5f, 100.0f, 0.05f, 0.0f);
	nOrientations = 9;
	gratingOrientations.clear();
	for (int i = 0; i < nOrientations; ++i)
		gratingOrientations.push_back(i*3.1458f / (float)(nOrientations-1));
	currentOrientationIndex = 0;
	gratingParameters.x = gratingOrientations[currentOrientationIndex];
	shuffle(gratingOrientations.begin(), gratingOrientations.end(),randomEngine);
}
void ExperimentManager::UpdateSandbox(unsigned int& frameCount)
{
	stim.InitRenderData();
	if (isDrifting)
		gratingParameters.w += 0.1f;

	if (isAlternating && frameCount > alternationTime)
	{
		isVisible = !isVisible;
		frameCount = 0;
		if (isVisible)
		{
			gratingParameters.x = gratingOrientations[currentOrientationIndex];
			++currentOrientationIndex;
			if (currentOrientationIndex == nOrientations)
			{
				currentOrientationIndex = 0;
				shuffle(gratingOrientations.begin(), gratingOrientations.end(), randomEngine);
			}
		}
	}
}
void ExperimentManager::DrawSandbox()
{
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (isVisible)
	{
		glUseProgram(apertureShader);
		glUniformMatrix4fv(glGetUniformLocation(apertureShader, "projection"), 1, GL_FALSE, glm::value_ptr(orthoMatrix));
		glUniform4fv(glGetUniformLocation(apertureShader, "gratingParameters"), 1, glm::value_ptr(gratingParameters));
		stim.Draw();
	}
}

void ExperimentManager::ProcessInput()
{
	if (keyStates[GLFW_KEY_D])
	{
		keyStates[GLFW_KEY_D] = false;
		isDrifting = !isDrifting;
	}

	if (keyStates[GLFW_KEY_S])
	{
		keyStates[GLFW_KEY_S] = false;
		isAlternating = !isAlternating;
	}

	stim.SetCenter(mouseX, mouseY);

	if (scrollDirection != 0)
	{
		stim.SetSize(stim.GetWidth() + 10 * scrollDirection, stim.GetHeight() + 10 * scrollDirection);
		scrollDirection = 0;
	}
}