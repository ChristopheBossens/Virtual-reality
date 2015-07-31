% Script that demonstrates how to read out the data collected from the
% photocol. The data file consists of 5-byte packages.
% Byte 1    : tag information
% Byte 2-3  : Timestamp
% Byte 4-5  : Voltage measurement

filename = 'C:\VR_SYSTEM\Data\sensor.bin';
fileInfo = dir(filename);
nPackets = fileInfo.bytes/5;

discardPackets = 100;
discardBytes = discardPackets*5;
dataPackets = nPackets-discardPackets;
data = zeros(3,dataPackets);

fid = fopen(filename);
fseek(fid,discardBytes,-1);
for packetIndex = discardPackets+1:nPackets
    tag = fread(fid,1);
    time = fread(fid,1,'uint16');
    value = fread(fid,1,'uint16');
    
    data(:,packetIndex-discardPackets) = [tag; time; value];
end
fclose(fid);

%% Plot results
timeVector = [0 diff(data(2,:))];
timeVector(timeVector < -60000) = timeVector(timeVector<-60000) + 65536;
timeVector = cumsum(timeVector);
plot(1:dataPackets,data(3,:))
set(gca,'YLim',[0 1024])
