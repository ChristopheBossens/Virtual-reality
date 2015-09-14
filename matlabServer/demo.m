% Example script illustrating how to use the experiment manager
clc;clear all;

% Initialize objects
experimentManager = ExperimentManager();
experimentManager.InitScreen(0);
experimentManager.InitPeripherals();


experimentManager.LoadXML('C:\VR_SYSTEM\test.xml')
experimentManager.PrepareTextures()

experimentManager.RunExperiment()
clear experimentManager
% Finish and clean up after the experiment
Screen('CloseAll');