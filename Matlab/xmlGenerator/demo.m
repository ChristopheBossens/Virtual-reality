% Demo script illustrating how an experiment can be created
clc;clear;
cd 'C:\Users\Christophe\Documents\GitHub\Virtual-reality\Matlab\xmlParser';
addpath('C:\Users\Christophe\Documents\GitHub\Virtual-reality\Matlab\xmlParser');

% First create a standard reward type and transition type
noneReward = Reward('none');
velocityReward = Reward('velocity');
timedTransition = Transition('duration');

% Create a list of stimuli that we will use during the experiment
gaborStimulus = Stimulus('gabor_grating');
apertureStimulus = Stimulus('aperture_grating');
targetStimulus = Stimulus('image');
targetStimulus.SetStimulusLocation('C:\Image.jpg');

% Create several trials
trialA = Trial(noneReward,timedTransition, gaborStimulus);
trialB = Trial(noneReward,timedTransition,apertureStimulus);
trialC = Trial(noneReward,timedTransition,targetStimulus);

% Create different trial blocks
trialBlockA = TrialBlock('random');
for i = 1:6
    switch mod(i,3)
        case 0
            trialBlockA.AddTrial(trialA);
        case 1
            trialBlockA.AddTrial(trialB);
        case 3
            trialBlockA.AddTrial(trialC);
    end
end

trialBlockB = TrialBlock('fixed');
for i = 1:6
    switch (mod(i,2))
        case 0
            trialBlockB.AddTrial(trialB);
        case 1
            trialBlockB.AddTrial(trialC);
    end
end

% Add the blocks to an aexperiment
experiment = Experiment;
experiment.AddBlock(trialBlockB);
experiment.AddBlock(trialBlockA);

experiment.WriteXML();