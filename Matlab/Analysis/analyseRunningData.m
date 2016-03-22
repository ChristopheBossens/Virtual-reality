% Reads in track data from the running ball
% The data file is a binary file in which data is coded in the following
% way
% 4 bytes: unsigned integer containing the frame number
% 8 bytes: double containing the time since the last frame
% 8 bytes: double containing the displacement in the x direction since the 
%          last frame
% 8 bytes: double containing the displacement in the y direction since the
%          last frame
% 1 byte: reward information
%          bit 1: one if a reward is given, zero otherwise
%          bit 5-8: contain information about the reward state 
%           0: wait for motion
%           1: reward for motion
%           2: wait for stationary
%           3: reward for stationary
% 2 bytes: Reserverd for trial numbers
% 1 byte : Reserverd for condition information
%
% The following values are calculated to make analysis more
% straightforward
%
% frameNumber       : contains the frame number
% frameDelta        : time since last frame was updated
% xData             : forward or backward motion
% yData             : sideways motion (not used)
% rewardVector      : contains one each time a reward is delivered
% timeVector        : accumulates the frameDelta values
% rewardStateVector : extracts the reward state information
% velocityVector    : Calculates current running velocity (cm/s)
clc;clear all
filepath ='G:\Projects\VR_SYSTEM\System\3D engine\Debug\DATA\20160322\';
currentFile = '68571r0';

if exist([filepath currentFile],'file') == 0
    error('File does not exist!');
end
fileInfo = dir([filepath currentFile]);

nPackets = fileInfo.bytes/32;

fid = fopen([filepath currentFile]);

frameNumber = zeros(1,nPackets);
frameDelta = zeros(1,nPackets);
xData = zeros(1,nPackets);
zData = (zeros(1,nPackets));
packetData = uint32(zeros(1,nPackets));
for sampleIndex = 1:nPackets
    framecount = fread(fid,1,'uint32');
    timeDelta = fread(fid,1,'double');
    xDelta = fread(fid,1,'double');
    zDelta = fread(fid,1,'double');
    dataPacket = uint32(fread(fid,1,'uint32'));
    
    frameNumber(sampleIndex) = framecount;
    frameDelta(sampleIndex) = timeDelta;
    xData(sampleIndex) = -1.*xDelta;
    zData(sampleIndex) = zDelta;
    packetData(sampleIndex) = dataPacket;
end


rewardVector = bitget(packetData,1);
timeVector = cumsum(frameDelta);
rewardStateVector = bitand(bitshift(packetData,-4),255);
velocityVector = xData./frameDelta;

fclose(fid);
%% Obtain frame statistics
ifiBins = [0:0.001:0.04];
ifiHist = histc(frameDelta,ifiBins);

clf,
subplot(3,1,1),bar(ifiBins,ifiHist)
subplot(3,1,2),plot(frameDelta)

%% Analysis of running speed
% Percentage time running
thresholdVelocity = 2.0;
smoothedVelocity = smooth(velocityVector,30)';

startIndex = 1;
stopIndex = 1;
nRunningBlocks = 0;
runningBlockDuration = zeros(1,nPackets);
runningBlockVelocity = zeros(1,nPackets);
while startIndex < nPackets && stopIndex < nPackets
    while startIndex < nPackets
        if smoothedVelocity(startIndex) > thresholdVelocity
            break;
        else
            startIndex = startIndex + 1;
        end
    end
    if startIndex == nPackets
        break;
    end
    stopIndex = startIndex + 1;
    while stopIndex < nPackets
        if smoothedVelocity(stopIndex) < thresholdVelocity
            break;
        else
            stopIndex = stopIndex + 1;
        end
    end
    nRunningBlocks = nRunningBlocks + 1;
    runningBlockDuration(nRunningBlocks) = timeVector(stopIndex)-timeVector(startIndex);
    runningBlockVelocity(nRunningBlocks) = mean(smoothedVelocity(startIndex:stopIndex));
    startIndex = stopIndex + 1;
end
runningBlockDuration = runningBlockDuration(1:nRunningBlocks);
runningBlockVelocity = runningBlockVelocity(1:nRunningBlocks);


runningBlockBins = [0:1:max(runningBlockDuration)];
runningVelocityBins = [0:0.5:max(runningBlockVelocity)]
runningBlockHist = histc(runningBlockDuration, runningBlockBins);
runningVelocityHist = histc(runningBlockVelocity,runningVelocityBins);

clf,
subplot(2,3,2),plot(runningBlockDuration,runningBlockVelocity,'.'),xlabel('Average velocity (cm/s)'),ylabel('Block duration (s)'),title('Velocity vs block duration')
hold on,
subplot(2,2,3),bar(runningBlockBins,runningBlockHist,1,'grouped'),xlabel('Block duration (s)'),ylabel('Count'),title('Block duration histogram')
subplot(2,2,4),bar(runningVelocityBins,runningVelocityHist,1,'grouped'),xlabel('Average block velocity (cm/s)'),ylabel('Count'),title('Average velocity histogram')


%% Analysis of reward for motion
% Get the number of rewards and average velocity
maxBlocks = 200;
rfmVelocity = zeros(1,maxBlocks);
startIndex = 1;
stopIndex = 1;
blockIndex = 0;
while (startIndex < nPackets) && (stopIndex < nPackets)
    while startIndex < nPackets
        if rewardStateVector(startIndex) == 1
            break;
        else
            startIndex = startIndex + 1;
        end
    end
    
    if startIndex == nPackets
        break;
    end
    
    stopIndex = startIndex + 1;
    while stopIndex < nPackets
        if rewardStateVector(stopIndex) == 1
            stopIndex = stopIndex + 1;
        else
            break;
        end
    end
    
    blockIndex = blockIndex + 1;
    rfmVelocity(blockIndex) = mean(smoothedVelocity(startIndex:stopIndex));
    startIndex = stopIndex + 1;
end

rfmVelocity = rfmVelocity(1:blockIndex);

% Analysis of reward for stationary
rfsVelocity = zeros(1,maxBlocks);
startIndex = 1;
stopIndex = 1;
blockIndex = 0;
while (startIndex < nPackets) && (stopIndex < nPackets)
    while startIndex < nPackets
        if rewardStateVector(startIndex) == 3
            break;
        else
            startIndex = startIndex + 1;
        end
    end
    
    if startIndex == nPackets
        break;
    end
    
    stopIndex = startIndex + 1;
    while stopIndex < nPackets
        if rewardStateVector(stopIndex) == 3
            stopIndex = stopIndex + 1;
        else
            break;
        end
    end
    
    blockIndex = blockIndex + 1;
    rfsVelocity(blockIndex) = mean(smoothedVelocity(startIndex:stopIndex));
    startIndex = stopIndex + 1;
end

rfsVelocity = rfsVelocity(1:blockIndex);
% Plot the data
figure,
boxplot([rfmVelocity' rfsVelocity']),hold on
plot(ones(1,length(rfmVelocity))+0.025.*randn(1,length(rfmVelocity)),rfmVelocity,'.k')
plot(2.*ones(1,length(rfmVelocity))+0.025.*randn(1,length(rfsVelocity)),rfsVelocity,'.k')
set(gca,'XTick',[1 2],'XTickLabel',{'Motion rewarded','Stationary rewarded'})
ylabel('Average velocity (cm/s)')
title('Average velocity during reward stationary blocks')