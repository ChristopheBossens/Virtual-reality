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