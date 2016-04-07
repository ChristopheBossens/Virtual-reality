% Requires the function ExtractRunningData
% Performs three different analyses:
% 1. Histogram of running and stationary block duration and velocity across
%    the complete session
% 2. Evolution of ratio running/stationary over the session
% 3. A windowed histogram for running and stationary block duration and
%    velocity
clc;clear all
showPlots = 1;

filepath ='E:\Data\Running_ball_V1_Population\Pilot_running_behaviour\Data\20160406\';
currentFile = '2016-april-06-rat68432-1012';

display('Reading file')
[frameNumber, frameDelta, xData, zData, rewardVector, timeVector, rewardStateVector,velocityVector] = ...
    ExtractRunningData(filepath, currentFile);
thresholdVelocity = 1.0;
smoothedVelocity = smooth(velocityVector,30)';
nFrames = length(frameNumber);

%% Comparative analysis with Niel and stryker paper:
% Histogram of velocities across the session, for this we average the
% velocity across each six frames
binnedVelocityVector = zeros(1,floor(length(velocityVector)/6));
for i = 1:(floor(length(velocityVector)/6))
    binnedVelocityVector(i) = mean(velocityVector( (6*(i-1)+1):(6*i)));
end

edges = [0:1:50];
edgeCount = histc(binnedVelocityVector,edges);
edgeCount = 100.*edgeCount./sum(edgeCount);
clf,
subplot(1,1,1),bar(edges,edgeCount,1)
xlabel('Velocity')
ylabel('% of time')

% valid trials
trialLength = 15; % One unit corresponds to 100 ms
runningTrials = zeros(1,floor(length(binnedVelocityVector)/trialLength));
for i = 1:length(runningTrials)
   if mean(binnedVelocityVector( (trialLength*(i-1)+1):(trialLength*i))) > 2
       runningTrials(i) = 1;
   else
       runningTrials(i) = 0;
   end
end

edges = [0:1:50];
edgeCount = histc(binnedVelocityVector,edges);
edgeCount = 100.*edgeCount./sum(edgeCount);
clf,
subplot(2,1,1),bar(edges,edgeCount,1)
xlabel('Velocity cm(s)')
ylabel('% of time')
title('Distribution of rat speed')
subplot(2,1,2),bar([0 1],[length(runningTrials)-sum(runningTrials) sum(runningTrials)])
set(gca,'XTick',[0 1],'XTickLabel',{'Stationary', 'Running'})
title(['Distribution of behavioral states (trial = ' num2str(100*trialLength) ' ms)'])
%% Analysis of running speed across the complete session
% Calculates histograms for block duration and velocity 
display('Analysing session running data')
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

velocityPoints = 0:1:30;
stationaryVelocityHist = histc(stationaryBlockVelocity,velocityPoints);
runningVelocityHist = histc(runningBlockVelocity,velocityPoints);

if showPlots == 1
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
end

ratData.sessionDurationPoints = durationPoints;
ratData.sessionVelocityPoints = velocityPoints;
ratData.sessionStationaryDurationHist = stationaryDurationHist;
ratData.sessionRunningDurationHist = runningDurationHist;
ratData.sessionStationaryVelocityHist = stationaryVelocityHist;
ratData.sessionRunningVelocityHist = runningVelocityHist;

%% Windowed ratio of running vs stationary
% A window of length 'windowLength' is slided over the data and in this window
% the ratio of running vs stationary periods is counted
display('Compare running vs stationary data across the session')
windowLength = 40*60; % 40 seconds times 60 frames per second

startIndex = 1;
stopIndex = stopIndex + windowLength -1;
runningRatio = zeros(1,nFrames-windowLength);
for i = 1:(nFrames-windowLength)
    stationaryPackets = length(find(smoothedVelocity(i:(i+windowLength-1))<thresholdVelocity));
    runningPackets = windowLength-stationaryPackets;
    
    runningRatio(i) = runningPackets/(runningPackets+stationaryPackets);
end

if showPlots == 1
    clf,
    plot(1:(nFrames-windowLength),runningRatio),
    line([1 nFrames-windowLength],[0.5 0.5],'LineStyle','--','Color','k')
    set(gca,'XTick',[1 round((nFrames-windowLength)/2) nFrames-windowLength],'XTickLabel', timeVector([1 round((nFrames-windowLength)/2) nFrames-windowLength])./60)
    set(gca,'YLim',[0 1])
    title(['Running versus stationary ratio (window=' num2str(windowLength/60) 's)'])
    xlabel('Time (m)')
    ylabel('running/(running+stationary)')
end
ratData.runningRatio = runningRatio;

%% Distribution changes over time
% In each window of length 'windowLength' we calculate the distribution of
% stationary block duration and running block durations. The velocity
% distribution for each behavioral state is also calculated.
% The 'stepSize' parameter defines how many frames the window is shifted
% for each subsequent calculation
display('Analysing windowed block duration and velocities')
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

if showPlots == 1
    clf,
    subplot(2,2,1),imshow(stationaryDurationHist,[]),
    c = colorbar;
    axis on
    set(gca,'Ydir','normal','YLim',[min(durationPoints(1))+0.5 max(durationPoints)])
    set(gca,'XTick',[1 round(nWindows./2) nWindows],'XTickLabel',round(timeVector([1 round((nFrames-windowLength)/2) (nFrames-windowLength)])./60))
    set(get(c,'YLabel'),'String','Count')
    title('subthreshold block duration histogram')
    xlabel('Time (m)')
    ylabel('Block duration (s)')

    subplot(2,2,2),imshow(runningDurationHist,[min(durationPoints) max(durationPoints)]),
    c = colorbar;
    axis on
    set(gca,'Ydir','normal','YLim',[min(durationPoints(1))+0.5 max(durationPoints)])
    set(gca,'XTick',[1 round(nWindows./2) nWindows],'XTickLabel',round(timeVector([1 round((nFrames-windowLength)/2) (nFrames-windowLength)])./60))
    set(get(c,'YLabel'),'String','Count')
    title('suprathreshold block duration histogram')
    xlabel('Time (m)')
    ylabel('Block duration (s)')

    subplot(2,2,3),imshow(stationaryVelocityHist, [min(velocityPoints) max(velocityPoints)]),
    c = colorbar;
    axis on
    set(gca,'Ydir','normal','YLim',[min(velocityPoints(1))+0.5 max(velocityPoints)])
    set(gca,'XTick',[1 round(nWindows./2) nWindows],'XTickLabel',round(timeVector([1 round((nFrames-windowLength)/2) (nFrames-windowLength)])./60))
    set(get(c,'YLabel'),'String','Count')
    title('Windowed stationary block velocity histogram')
    xlabel('Time (m)')
    ylabel('Block duration (s)')

    subplot(2,2,4),imshow(runningVelocityHist, [min(velocityPoints) max(velocityPoints)]),
    c = colorbar;
    axis on
    set(gca,'Ydir','normal','YLim',[min(velocityPoints(1))+0.5 max(velocityPoints)])
    set(gca,'XTick',[1 round(nWindows./2) nWindows],'XTickLabel',round(timeVector([1 round((nFrames-windowLength)/2) (nFrames-windowLength)])./60))
    set(get(c,'YLabel'),'String','Count')
    title('Windowed running block velocity histogram')
    xlabel('Time (m)')
    ylabel('Block duration (s)')
    colormap jet
end
ratData.windowStationaryDurationHist = stationaryDurationHist;
ratData.windowRunningDurationHist = runningDurationHist;
ratData.windowStationaryVelocityHist = stationaryVelocityHist;
ratData.windowRunningVelocityHist = runningVelocityHist;

save([filepath currentFile '.mat'], '-struct','ratData');
display('Saving and done...')
