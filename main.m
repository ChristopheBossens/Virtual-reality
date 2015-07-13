% Main script for getting information on each screen
clc;clear all;

% Default parameters
clearColor = [0.2 0.2 0.2];

% Enable left and right monitor
lMonitorIndex = 2;
rMonitorIndex = 3;

[lMonitorWindow, lMonitorRect] = Screen('OpenWindow',lMonitorIndex, clearColor);
[rMonitorWindow, rMonitorRect] = Screen('OpenWindow',rMonitorIndex, clearColor);

% Show some statistics
lMonitorIFI = Screen('GetFlipInterval',lMonitorWindow);
rMonitorIFI = Screen('GetFlipInterval',lMonitorWindow);

lMonitorHz = 1/lMonitorIFI;
rMonitorHz = 1/rMonitorIFI;

lMonitorNominalHz = Screen('NominalFrameRate',lMonitorWindow);
rMonitorNominalHz = Screen('NominalFrameRate',rMonitorWindow);

[lMonitorWidth, lMonitorHeight] = Screen('DisplaySize', lMonitorWindow);
[rMonitorWidth, rMonitorHeight] = Screen('DisplaySize', rMonitorWindow);
[lMonitorPixelsX, lMonitorPixelsY] = Screen('WindowSize',lMonitorWindow);
[rMonitorPixelsX, rMonitorPixelsY] = Screen('WindowSize',rMonitorWindow);

% First upload stimuli to stimulus manager, which takes care of loading
stimulusManager = StimulusManager(lMonitorWindow, rMonitorWindow);
stimulusPath = 'C:\VR_SYSTEM\Stimuli\';
sIndex1 = stimulusManager.AddStimulus('background',[stimulusPath 'background.png']);
sIndex2 = stimulusManager.AddStimulus('grating1',[stimulusPath 'stimulus_1.png']);
sIndex3 = stimulusManager.AddStimulus('grating2',[stimulusPath 'stimulus_2.png']);
sIndex4 = stimulusManager.AddStimulus('testdrift',[stimulusPath 'stimulus_0.png']);
% Create presentation data for testing
% Col 1: monitor
% Col 2: stimulus presentation order
% Col 3: stimulus id
% Col 4: presentation time in frames
% Col 5: remaining columns can be used to provide texture offset data
stimulusData = [1 1 sIndex1 80;
                1 2 sIndex2 90;
                1 3 sIndex1 70;
                2 1 sIndex2 50;
                2 2 sIndex3 90;
                2 3 sIndex2 70];
            
% Create frame parameter list for motion independent experiment
frameParameters = stimulusManager.GenerateFramesFromCounts(stimulusData);



% Enable hardware devices

% Run the experiment
for frameIndex = 1:nFrames
    % Readvelocity
    % Update function
    if ( frameParameters(frameIndex,1) == 0)
        Screen('FillRect',lMonitorWindow,clearColor);
    else
        Screen('DrawTexture', lMonitorWindow, frameParameters(frameIndex,1),[],[],0);
    end
    
    if ( frameParameters(frameIndex,2) == 0)
        Screen('FillRect',rMonitorWindow,clearColor);
    else
        Screen('DrawTexture', rMonitorWindow, frameParameters(frameIndex,2),[],[],0);
    end
    % Drawing function
    Screen('Flip',lMonitorWindow);
    Screen('Flip',rMonitorWindow);
end

Screen('FillRect',lMonitorWindow, clearColor);
Screen('FillRect',rMonitorWindow, clearColor);
Screen('Flip',lMonitorWindow);
Screen('Flip',rMonitorWindow);

% Finish and clean up after the experiment
Screen('CloseAll');