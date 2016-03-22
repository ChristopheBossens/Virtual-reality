#pragma once

#include "tinyxml2.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace tinyxml2;
using namespace std;

enum STIM_TYPE
{
	GABOR_GRATING,
	APERTURE_GRATING,
	IMAGE,
	VIDEO
};

enum EVENT_TYPE
{
	NONE,
	VELOCITY,
	FRAME_COUNT,
	INTERVAL
};

const int MAX_PARAMS = 10;
struct RewardEvent
{
	EVENT_TYPE event;
	int parameters[MAX_PARAMS];
};

struct TransitionEvent
{
	EVENT_TYPE event;
	int parameters[MAX_PARAMS];
};

struct StimulusListElement
{
	STIM_TYPE   stimulusType;
	std::string stimulusName;
	std::string stimulusLocation;
	unsigned int texID;
};

struct TrialStimulus
{
	StimulusListElement* stimulusListElement;
	int stimParams[MAX_PARAMS];

	double extraParams[MAX_PARAMS];
	bool frameDependent[MAX_PARAMS];
};

struct TrialListElement
{
	int id;
	int condition;

	vector<TrialStimulus> trialStimuli;
	RewardEvent rewardEvent;
	TransitionEvent transitionEvent;
};

class ExperimentLoader
{
private:
	XMLError xmlError;
	XMLNode* rootNode;
	XMLElement* stimulusListElement;
	XMLElement* trialListElement;


	void ParseStimulusList();
	void ParseStimulus(XMLElement* stimulusElement);

	void ParseTrialList();
	void ParseTrial(XMLElement* trialElement);
	TrialStimulus ParseTrialStimulus(XMLElement* trialStimulus);
	RewardEvent ParseTrialReward(XMLElement* trialReward);
	TransitionEvent ParseTrialTransition(XMLElement* trialTransition);

public:
	vector<TrialListElement> trialList;
	std::map<string, StimulusListElement> stimulusList;

	void LoadExperiment(std::string filename);
	void PrintStimulusData();
	void PrintTrialData();
};