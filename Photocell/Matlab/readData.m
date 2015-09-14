% Script that demonstrates how to read out the data collected from the
% photocol. The data file consists of 5-byte packages.
% Byte 1    : tag information
% Byte 2-3  : Timestamp
% Byte 4-5  : Voltage measurement
clc;clear all;

filename = 'F:\Projects\VirtualReality\Data\linuxRightScreen.bin';
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

%% Plot results
% Here we convert the time vector to millisecond unit scale and the sensor data
% to voltage unit scale 
tagPositions = find(data(1,:) == 5);
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

%% Analyse photocell trace
% Thresholds are detected if the voltage trace is above threshold and if
% the rate of change of the trace is positive. If the difference between
% the previous threshold detection is smaller than 4 ms we discard the
% detection (because the refresh rate is 60 Hz, any difference lower than
% 16 ms is impossible
threshold = 1.5;
startTime = find(timeVector>7448,1);
stopTime  = find(timeVector>6.068e5,1);

onsetIndices = [];
firstPass = 0;
for i = startTime:stopTime
    if voltageVector(i) > threshold && firstPass == 0 && voltageDelta(i-1) > 0
        onsetIndices = [onsetIndices i];
        if length(onsetIndices) > 1 && (timeVector(onsetIndices(end))-timeVector(onsetIndices(end-1)) < 4)
            onsetIndices = onsetIndices(1:(end-1));
        end
        firstPass = 1;
    end
    if voltageVector(i) < threshold
        firstPass = 0;
    end
end
onsetDelta = diff(timeVector(onsetIndices));

lineX = repmat(timeVector(onsetIndices),2,1);
lineY = repmat([0;1],1,length(onsetIndices));
figure('color','w'), hold on
subplot(2,1,1)
plot(timeVector,voltageVector)
line([timeVector(1) timeVector(end)],[threshold threshold],'color',[1 0 0])
line(lineX,lineY)
subplot(2,1,2)
plot(linspace(0,1,length(onsetDelta)),onsetDelta,'.')
set(gca,'XTick',[])
title('Time between onset threshold triggers')

