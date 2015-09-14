clc;clear all;
% Test stereo mode
stereoMode = 0;
Screen('Preference','SkipSyncTests',1);
PsychImaging('PrepareConfiguration');

[windowPtr, windowRect] = PsychImaging('OpenWindow',0,[],[],[],stereoMode);

nFlips = 300;
dataMatrix = zeros(nFlips,7);
tic
for i = 1:nFlips
    if mod(i,60) == 0
        clearColor = [255 255 255];
    else
        clearColor = [0 0 0];
    end
    
    %Screen('SelectStereoDrawBuffer',windowPtr,0);
    Screen('FillRect',windowPtr,clearColor);
    Screen('FillRect',windowPtr,[100 0 0],[20,20,60,60]);
    Screen('FillRect',windowPtr,[0 0 100],[3000,20,3060,60]);
    %Screen('SelectStereoDrawBuffer',windowPtr,1);
    %Screen('FillRect',windowPtr,clearColor);
    
    startTime = toc;
    [VLB SO FT M B] = Screen('Flip',windowPtr);
    stopTime = toc;
    
    dataMatrix(i,:) = [VLB SO FT M B startTime stopTime];
end
Screen('CloseAll');
%%
clc;clear all;
stereoMode = 10;

[window1,windowRect1] = Screen('OpenWindow',1);
[window2,windowRect1] = Screen('OpenWindow',2);

nFlips = 300;
for i = 1:nFlips
    if mod(i,60) == 0
        clearColor = [255 255 255];
    else
        clearColor = [0 0 0];
    end
    
    Screen('FillRect',window1,clearColor);
    Screen('FillRect',window2,clearColor);
    Screen('Flip',window1);
    Screen('Flip',window2);
end
Screen('CloseAll');
