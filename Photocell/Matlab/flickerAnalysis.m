% Script that demonstrates how to read out the data collected from the
% photocol. The data file consists of 5-byte packages.
% Byte 1    : tag information
% Byte 2-3  : Timestamp
% Byte 4-5  : Voltage measurement
clc;clear all;

filename = 'F:\Projects\VirtualReality\Data\screenOscillations.bin';
fileInfo = dir(filename);
nPackets = fileInfo.bytes/5;

discardPackets = 1000;
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
%%
timeVector = [0 diff(data(2,:))];
timeVector(timeVector < -60000) = timeVector(timeVector<-60000) + 65536;
timeVector = cumsum(timeVector)./1000;

voltageVector = 5.*data(3,:)./1024;
i = find(voltageVector > 6);
voltageVector(i) = voltageVector(i-1);
voltageDelta = [0 diff(voltageVector)];

% Plot full trace + zoom in + difference
figure('color','w')
subplot(2,1,1),plot(timeVector,voltageVector)
box off
xlabel('Time [ms]')
ylabel('Voltage [v]')
title('Full trace')

subplot(2,1,2),hold on
h1 = plot(timeVector(16000:20500),voltageVector(16000:20500))
h2 = plot(timeVector(16000:20500),voltageDelta(16000:20500),'r')
legend([h1 h2],'Voltage trace', 'Voltage difference','Location','SouthEast')
box off
xlabel('Time [ms]')
ylabel('Voltage [v]')
set(gca,'YLim',[-3 5])

%% Analyse frequency spectrum of the signal
vMin = min(voltageVector);
vMax = max(voltageVector);
vRange = vMax - vMin;
voltageVector = -1+(voltageVector-vMin)./(vRange/2);
sampleRate = 3571
nSamples  = length(voltageVector);
p = abs(fft(voltageVector));
f = (0:nSamples-1).*(sampleRate/nSamples);
maxFreq = find(f>150,1);
plot(f(1:maxFreq),p(1:maxFreq));
plot(f,p)