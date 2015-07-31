% Main script for getting information on each screen
clc;clear all;

% Initialize objects
experimentManager = ExperimentManager();
experimentManager.Init();
stimulusManager = StimulusManager(experimentManager.lMonitorWindow, experimentManager.rMonitorWindow);

% First upload stimuli to stimulus manager, which takes care of loading
stimulusPath = 'C:\VR_SYSTEM\Stimuli\';
sIndex1 = stimulusManager.AddStimulus('background',[stimulusPath 'black.png']);
sIndex2 = stimulusManager.AddStimulus('grating1',[stimulusPath 'white.png']);

% Create presentation data for testing
% Col 1: monitor
% Col 2: stimulus presentation order
% Col 3: stimulus id
% Col 4: requested presentation time in frames
% Col 5: remaining columns can be used to provide texture offset data
stimulusData = [1 1 sIndex1 30;
                1 2 sIndex2 5;
                1 3 sIndex1 5;
                1 4 sIndex2 5;
                1 5 sIndex1 5;
                1 6 sIndex2 5;
                
                1 7 sIndex1 30;
                1 8 sIndex2 1;
                1 9 sIndex1 1;
                1 10 sIndex2 1;
                1 11 sIndex1 1;
                1 12 sIndex2 1;
                
                1 13 sIndex1 30;
                1 14 sIndex2 1;
                1 15 sIndex1 5;
                1 16 sIndex2 1;
                1 17 sIndex1 5;
                1 18 sIndex2 1;
                
                1 19 sIndex1 30;
                1 20 sIndex2 1;
                1 21 sIndex1 20;
                1 22 sIndex2 1;
                1 23 sIndex1 20;
                1 24 sIndex2 1;
                1 25 sIndex1 30;
                
                2 1 sIndex1 30;
                2 2 sIndex2 5;
                2 3 sIndex1 5;
                2 4 sIndex2 5;
                2 5 sIndex1 5;
                2 6 sIndex2 5;
                
                2 7 sIndex1 30;
                2 8 sIndex2 1;
                2 9 sIndex1 1;
                2 10 sIndex2 1;
                2 11 sIndex1 1;
                2 12 sIndex2 1;
                
                2 13 sIndex1 30;
                2 14 sIndex2 1;
                2 15 sIndex1 5;
                2 16 sIndex2 1;
                2 17 sIndex1 5;
                2 18 sIndex2 1;
                
                2 19 sIndex1 30;
                2 20 sIndex2 1;
                2 21 sIndex1 20;
                2 22 sIndex2 1;
                2 23 sIndex1 20;
                2 24 sIndex2 1;
                2 25 sIndex1 30];           
frameParameters = stimulusManager.GenerateFramesFromCounts(stimulusData);

experimentManager.AttachFrames(frameParameters);

experimentManager.RunFrames();

% Finish and clean up after the experiment
Screen('CloseAll');