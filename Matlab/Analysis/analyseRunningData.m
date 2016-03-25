% Requires the function ExtractRunningData
% Performs three different analyses:
% 1. Histogram of running and stationary block duration and velocity across
%    the complete session
% 2. Evolution of ratio running/stationary over the session
% 3. A windowed histogram for running and stationary block duration and
%    velocity
clc;clear all
filepath ='E:\Data\Running_ball_V1_Population\Pilot_running_behaviour\Data\20160325\';
currentFile = '68680r0';

[frameNumber, frameDelta, xData, zData, rewardVector, timeVector, rewardStateVector,velocityVector] = ...
    ExtractRunningData(filepath, currentFile);
thresholdVelocity = 2.0;
smoothedVelocity = smooth(velocityVector,30)';
nFrames = length(frameNumber);
%% Analysis of running speed across the complete session
% Calculates histograms for block duration and velocity 
thresholdVelocity = 2.0;
nFrames = length(frameNumber);
runningBlockDuration = zeros(1,nFrames);
runningBlockVelocity = zeros(1,nFrames);
stationaryBlockDuration = zeros(1,nFrames);
stationaryBlockVelocity = zeros(1,nFrames);

startIndex = 1;
stopIndex = 1;
stationaryBlockIdx = 0;
runningBlockIdx = 0;
while startIndex < nFrames && stopIndex < nFrames
    % Stationary period: start looking for start of suprathreshold velocity
    if smoothedVelocity(stopIndex) < thresholdVelocity
        while stopIndex < nFrames
            if smoothedVelocity(stopIndex) < thresholdVelocity
                stopIndex = stopIndex + 1;
            else
                break
            end
        end
        
        stationaryBlockIdx = stationaryBlockIdx + 1;
        stationaryBlockDuration(stationaryBlockIdx) = timeVector(stopIndex)-timeVector(startIndex);
        stationaryBlockVelocity(stationaryBlockIdx) = mean(smoothedVelocity(startIndex:stopIndex));
        
        startIndex = stopIndex;
    end
    
    % Running period: start looking for start of subthreshold velocity
    if smoothedVelocity(stopIndex) >= thresholdVelocity
        while stopIndex < nFrames
            if smoothedVelocity(stopIndex) >= thresholdVelocity
                stopIndex = stopIndex + 1;
            else
                break
            end
        end
        
        runningBlockIdx = runningBlockIdx + 1;
        runningBlockDuration(runningBlockIdx) = timeVector(stopIndex) - timeVector(startIndex);
        runningBlockVelocity(runningBlockIdx) = mean(smoothedVelocity(startIndex:stopIndex));
        
        startIndex = stopIndex;
    end
end

stationaryBlockDuration = stationaryBlockDuration(1:stationaryBlockIdx);
stationaryBlockVelocity = stationaryBlockVelocity(1:stationaryBlockIdx);
runningBlockVelocity = runningBlockVelocity(1:runningBlockIdx);
runningBlockDuration = runningBlockDuration(1:runningBlockIdx);

durationPoints = 0:1:20;
stationaryDurationHist = histc(stationaryBlockDuration,durationPoints);
runningDurationHist = histc(runningBlockDuration, durationPoints);

velocityPoints = 0:0.5:30;
stationaryVelocityHist = histc(stationaryBlockVelocity,velocityPoints);
runningVelocityHist = histc(runningBlockVelocity,velocityPoints);

clf,
subplot(2,2,1),bar(durationPoints,stationaryDurationHist,1),title('Stationary block duration histogram')
set(gca,'XLim',[min(durationPoints) max(durationPoints)])
xlabel('Block duration (s)')
ylabel('Count')
subplot(2,2,2),bar(durationPoints,runningDurationHist,1),title('Running block duration histogram')
set(gca,'XLim',[min(durationPoints) max(durationPoints)])
xlabel('Block duration (s)')
ylabel('Count')
subplot(2,2,3),bar(velocityPoints,stationaryVelocityHist,1),title('Stationary block average velocity histogram')
set(gca,'XLim',[min(velocityPoints) max(velocityPoints)])
xlabel('Block velocity (cm/s)')
ylabel('Count')
subplot(2,2,4),bar(velocityPoints,runningVelocityHist,1),title('Running block average velocity histogram')
set(gca,'XLim',[min(velocityPoints) max(velocityPoints)])
xlabel('Block velocity (cm/s)')
ylabel('Count')
%% Windowed ratio of running vs stationary
% A window of length 'windowLength' is slided over the data and in this window
% the ratio of running vs stationary periods is counted
windowLength = 40*60; % 40 seconds times 60 frames per second

startIndex = 1;
stopIndex = stopIndex + windowLength -1;
runningRatio = zeros(1,nFrames-windowLength);
for i = 1:(nFrames-windowLength)
    stationaryPackets = length(find(smoothedVelocity(i:(i+windowLength-1))<thresholdVelocity));
    runningPackets = windowLength-stationaryPackets;
    
    runningRatio(i) = runningPackets/(runningPackets+stationaryPackets);
end

clf,
plot(1:(nFrames-windowLength),runningRatio),
line([1 nFrames-windowLength],[0.5 0.5],'LineStyle','--','Color','k')
set(gca,'XTick',[1 round((nFrames-windowLength)/2) nFrames-windowLength],'XTickLabel', timeVector([1 round((nFrames-windowLength)/2) nFrames-windowLength])./60)
set(gca,'YLim',[0 1])
title(['Running versus stationary ratio (window=' num2str(windowLength/60) 's)'])
xlabel('Time (m)')
ylabel('running/(running+stationary)')
%% Distribution changes over time
% In each window of length 'windowLength' we calculate the distribution of
% stationary block duration and running block durations. The velocity
% distribution for each behavioral state is also calculated.
% The 'stepSize' parameter defines how many frames the window is shifted
% for each subsequent calculation
windowLength = 120*60;
stepSize = 30*60;
nWindows = length(1:stepSize:(nFrames-windowLength));

durationPoints = 0:1:20;
stationaryDurationHist =zeros(length(durationPoints),nWindows);
runningDurationHist = zeros(length(durationPoints),nWindows);

velocityPoints = 0:1:30;
stationaryVelocityHist = zeros(length(velocityPoints),nWindows);
runningVelocityHist = zeros(length(velocityPoints),nWindows);

windowIndex = 0;
for i = 1:stepSize:(nFrames-windowLength)
    
    nPoints = i + windowLength-1;
    startIndex = i;
    stopIndex = i;
    stationaryBlockIdx = 0;
    runningBlockIdx = 0;
    
    stationaryBlockDuration = zeros(1,windowLength);
    stationaryBlockVelocity = zeros(1,windowLength);
    runningBlockDuration = zeros(1,windowLength);
    runningBlockVelocity = zeros(1,windowLength);
    
    while startIndex < nPoints && stopIndex < nPoints
        % Stationary period: start looking for start of suprathreshold velocity
        if smoothedVelocity(stopIndex) < thresholdVelocity
            while stopIndex < nPoints
                if smoothedVelocity(stopIndex) < thresholdVelocity
                    stopIndex = stopIndex + 1;
                else
                    break
                end
            end

            stationaryBlockIdx = stationaryBlockIdx + 1;
            stationaryBlockDuration(stationaryBlockIdx) = timeVector(stopIndex)-timeVector(startIndex);
            stationaryBlockVelocity(stationaryBlockIdx) = mean(smoothedVelocity(startIndex:stopIndex));

            startIndex = stopIndex;
        end

        % Running period: start looking for start of subthreshold velocity
        if smoothedVelocity(stopIndex) >= thresholdVelocity
            while stopIndex < nPoints
                if smoothedVelocity(stopIndex) >= thresholdVelocity
                    stopIndex = stopIndex + 1;
                else
                    break
                end
            end

            runningBlockIdx = runningBlockIdx + 1;
            runningBlockDuration(runningBlockIdx) = timeVector(stopIndex) - timeVector(startIndex);
            runningBlockVelocity(runningBlockIdx) = mean(smoothedVelocity(startIndex:stopIndex));

            startIndex = stopIndex;
        end
    end
    
    windowIndex = windowIndex + 1;
    stationaryBlockDuration = stationaryBlockDuration(1:stationaryBlockIdx);
    stationaryBlockVelocity = stationaryBlockVelocity(1:stationaryBlockIdx);
    runningBlockVelocity = runningBlockVelocity(1:runningBlockIdx);
    runningBlockDuration = runningBlockDuration(1:runningBlockIdx);

    stationaryDurationHist(:,windowIndex) = histc(stationaryBlockDuration,durationPoints);
    runningDurationHist(:,windowIndex) = histc(runningBlockDuration, durationPoints);

    stationaryVelocityHist(:,windowIndex) = histc(stationaryBlockVelocity,velocityPoints);
    runningVelocityHist(:,windowIndex) = histc(runningBlockVelocity,velocityPoints);
end

clf,
subplot(2,2,1),imshow(stationaryDurationHist,[]),
c = colorbar;
axis on
set(gca,'Ydir','normal','YLim',[min(durationPoints(2)) max(durationPoints)])
set(gca,'XTick',[1 round(nWindows./2) nWindows],'XTickLabel',round(timeVector([1 round((nFrames-windowLength)/2) (nFrames-windowLength)])./60))
set(get(c,'YLabel'),'String','Count')
title('Windowed stationary block duration histogram')
xlabel('Time (m)')
ylabel('Block duration (s)')

subplot(2,2,2),imshow(runningDurationHist,[min(durationPoints) max(durationPoints)]),
c = colorbar;
axis on
set(gca,'Ydir','normal','YLim',[min(durationPoints(2)) max(durationPoints)])
set(gca,'XTick',[1 round(nWindows./2) nWindows],'XTickLabel',round(timeVector([1 round((nFrames-windowLength)/2) (nFrames-windowLength)])./60))
set(get(c,'YLabel'),'String','Count')
title('Windowed running block duration histogram')
xlabel('Time (m)')
ylabel('Block duration (s)')

subplot(2,2,3),imshow(stationaryVelocityHist, [min(velocityPoints) max(velocityPoints)]),
c = colorbar;
axis on
set(gca,'Ydir','normal','YLim',[min(velocityPoints(2)) max(velocityPoints)])
set(gca,'XTick',[1 round(nWindows./2) nWindows],'XTickLabel',round(timeVector([1 round((nFrames-windowLength)/2) (nFrames-windowLength)])./60))
set(get(c,'YLabel'),'String','Count')
title('Windowed stationary block velocity histogram')
xlabel('Time (m)')
ylabel('Block duration (s)')

subplot(2,2,4),imshow(runningVelocityHist, [min(velocityPoints) max(velocityPoints)]),
c = colorbar;
axis on
set(gca,'Ydir','normal','YLim',[min(velocityPoints(2)) max(velocityPoints)])
set(gca,'XTick',[1 round(nWindows./2) nWindows],'XTickLabel',round(timeVector([1 round((nFrames-windowLength)/2) (nFrames-windowLength)])./60))
set(get(c,'YLabel'),'String','Count')
title('Windowed running block velocity histogram')
xlabel('Time (m)')
ylabel('Block duration (s)')
colormap jet