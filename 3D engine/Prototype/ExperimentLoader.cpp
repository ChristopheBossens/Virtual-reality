#include "ExperimentLoader.h"


void ExperimentLoader::LoadExperiment(std::string filename)
{
	XMLDocument xmlDocument;
	xmlError = xmlDocument.LoadFile(filename.c_str());

	if (xmlError == XML_NO_ERROR)
	{
		rootNode = xmlDocument.FirstChild();
		if (rootNode == nullptr)
		{
			cout << "No valid entry point found" << endl;
			return;
		}

		stimulusListElement = rootNode->FirstChildElement("StimulusList");
		if(stimulusListElement == nullptr)
		{
			cout << "No stimulus list found" << endl;
			return;
		}
		ParseStimulusList();

		trialListElement = rootNode->FirstChildElement("TrialList");
		if (trialListElement == nullptr)
		{
			cout << "No trial list found" << endl;
			return;
		}
		ParseTrialList();
	}
	else
	{
		cout << "Error parsing xml file: " << endl;
		cout << xmlDocument.ErrorName() << endl;
	}
}

void ExperimentLoader::PrintStimulusData()
{
	std::map<string, StimulusListElement>::iterator it;
	for (it = stimulusList.begin(); it != stimulusList.end();++it)
	{
		cout << "Stimulus " << endl;
		cout << "Type: " << it->second.stimulusType << endl;
		cout << "Name: " << it->second.stimulusName << endl;
		cout << "Location: " << it->second.stimulusLocation << endl;
	}
	cout << endl;
}

void ExperimentLoader::PrintTrialData()
{
	for (size_t i = 0; i < trialList.size();++i)
	{
		cout << "Trial: " << i << endl;
		cout << "id: " << trialList[i].id << endl;
		cout << "cond: " << trialList[i].condition << endl;

		for(size_t j = 0; j < trialList[i].trialStimuli.size();++j)
		{
			cout << "Stimulus " << j + 1 << ": " << (trialList[i].trialStimuli[j].stimulusListElement)->stimulusName;
		}
	}
}
void ExperimentLoader::ParseStimulusList()
{
	XMLElement* stimulusElement = stimulusListElement->FirstChildElement("Stimulus");

	stimulusList.clear();
	while (stimulusElement != nullptr)
	{
		ParseStimulus(stimulusElement);
		stimulusElement = stimulusElement->NextSiblingElement();
	}
}
void ExperimentLoader::ParseStimulus(XMLElement* stimulusElement)
{
	const char* stimulusName, *stimulusType, *stimulusLocation;
	XMLElement* stimulusNameElement = stimulusElement->FirstChildElement("Name");
	XMLElement* stimulusTypeElement = stimulusElement->FirstChildElement("Type");
	if (stimulusNameElement == nullptr)
	{
		cout << "Stimulus has no name" << endl;
		return;
	}
	stimulusName = stimulusNameElement->GetText();

	if (stimulusTypeElement == nullptr)
	{
		cout << "Stimulus has no type" << endl;
		return;
	}
	stimulusType = stimulusTypeElement->GetText();
	
	StimulusListElement newStimulusData;
	newStimulusData.stimulusName  = string(stimulusName);
	if (string(stimulusType) == "APERTURE_GRATING")
	{
		newStimulusData.stimulusType = APERTURE_GRATING;
	}
	else if (string(stimulusType) == "IMAGE")
	{
		newStimulusData.stimulusType = IMAGE;
		XMLElement* stimulusLocationElement = stimulusElement->FirstChildElement("Location");
		if (stimulusLocationElement == nullptr)
		{
			cout << "Image location not found" << endl;
			return;
		}
		stimulusLocation = stimulusLocationElement->GetText();
		newStimulusData.stimulusLocation = string(stimulusLocation);
	}
	else if (string(stimulusType) == "GABOR_GRATING")
	{
		newStimulusData.stimulusType = GABOR_GRATING;
	}

	if (stimulusList.find(newStimulusData.stimulusName) == stimulusList.end())
		stimulusList[newStimulusData.stimulusName] = newStimulusData;
}


void ExperimentLoader::ParseTrialList()
{
	XMLElement* trialElement = trialListElement->FirstChildElement("Trial");

	trialList.clear();
	while (trialElement != nullptr)
	{
		ParseTrial(trialElement);
		trialElement = trialElement->NextSiblingElement();
	}
}
void ExperimentLoader::ParseTrial(XMLElement* trialElement)
{
	TrialListElement currentTrial;

	XMLElement* trialIdElement = trialElement->FirstChildElement("Id");
	XMLElement* trialCondElement = trialElement->FirstChildElement("Condition");
	XMLElement* trialReward = trialElement->FirstChildElement("Reward");
	XMLElement* trialTransition = trialElement->FirstChildElement("Transition");

	trialIdElement->QueryIntText(&currentTrial.id);
	trialCondElement->QueryIntText(&currentTrial.condition);
	currentTrial.rewardEvent = ParseTrialReward(trialReward);
	currentTrial.transitionEvent = ParseTrialTransition(trialTransition);

	XMLElement* trialStimulus = trialElement->FirstChildElement("Stimulus");
	while (trialStimulus != nullptr)
	{
		currentTrial.trialStimuli.push_back(ParseTrialStimulus(trialStimulus));
		trialStimulus = trialStimulus->NextSiblingElement("Stimulus");
	}
	trialList.push_back(currentTrial);
}

TrialStimulus ExperimentLoader::ParseTrialStimulus(XMLElement* trialStimulus)
{
	TrialStimulus currentTrialStimulus;
	XMLElement* stimParam;

	// Get stimulus name and check if it is in the stimulus list
	stimParam = trialStimulus->FirstChildElement("Name");
	std::map<string, StimulusListElement>::iterator it = stimulusList.find(string(stimParam->GetText()));
	if (it == stimulusList.end())
	{
		cout << "Error. Could not find stimulus '" << string(stimParam->GetText()) << " in stimulus list" << endl;
		return currentTrialStimulus;
	}
	else
		currentTrialStimulus.stimulusListElement = &(it->second);

	// Gather general stimulus parameters
	stimParam = trialStimulus->FirstChildElement("Top");
	stimParam->QueryIntText(&currentTrialStimulus.stimParams[0]);

	stimParam = trialStimulus->FirstChildElement("Left");
	stimParam->QueryIntText(&currentTrialStimulus.stimParams[1]);

	stimParam = trialStimulus->FirstChildElement("Width");
	stimParam->QueryIntText(&currentTrialStimulus.stimParams[2]);

	stimParam = trialStimulus->FirstChildElement("Height");
	stimParam->QueryIntText(&currentTrialStimulus.stimParams[3]);

	stimParam = trialStimulus->FirstChildElement("StartPresentation");
	stimParam->QueryIntText(&currentTrialStimulus.stimParams[4]);

	stimParam = trialStimulus->FirstChildElement("StopPresentation");
	stimParam->QueryIntText(&currentTrialStimulus.stimParams[5]);

	// Gather type specific parameters
	if (currentTrialStimulus.stimulusListElement->stimulusType == APERTURE_GRATING)
	{
		stimParam = trialStimulus->FirstChildElement("Orientation");
		stimParam->QueryDoubleText(&currentTrialStimulus.extraParams[0]);

		stimParam = trialStimulus->FirstChildElement("SpatialFrequency");
		stimParam->QueryDoubleText(&currentTrialStimulus.extraParams[1]);

		stimParam = trialStimulus->FirstChildElement("Phase");
		stimParam->QueryDoubleText(&currentTrialStimulus.extraParams[2]);

		stimParam = trialStimulus->FirstChildElement("Radius");
		stimParam->QueryDoubleText(&currentTrialStimulus.extraParams[3]);
	}
	return currentTrialStimulus;
}
RewardEvent ExperimentLoader::ParseTrialReward(XMLElement* trialReward)
{
	XMLElement* param;
	RewardEvent rewardEvent;

	if (trialReward != nullptr)
	{
		param = trialReward->FirstChildElement("Type");
		if (string(param->GetText()) == "Duration")
		{
			param = trialReward->FirstChildElement("FrameCount");

			rewardEvent.event = FRAME_COUNT;
			param->QueryIntText(&rewardEvent.parameters[0]);
		}
	}
	else
	{
		rewardEvent.event = NONE;
	}
	return rewardEvent;
}
TransitionEvent ExperimentLoader::ParseTrialTransition(XMLElement* trialTransition)
{
	XMLElement* param;
	TransitionEvent transitionEvent;
	
	param = trialTransition->FirstChildElement("Type");
	if (string(param->GetText()) == "Duration")
	{
		param = trialTransition->FirstChildElement("FrameCount");

		transitionEvent.event = FRAME_COUNT;
		param->QueryIntText(&transitionEvent.parameters[0]);
	}

	return transitionEvent;
}
