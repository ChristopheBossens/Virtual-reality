% Script that demonstrates how to read out the data collected from the
% photocol. The data file consists of 5-byte packages.
% Byte 1    : tag information
% Byte 2-3  : Timestamp
% Byte 4-5  : Voltage measurement

filename = 'C:\Users\Christophe\Documents\GitHub\Virtual-reality\Photocell\Calibration\long.bin';
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
tagPositions = find(data(1,:) == 5);
timeVector = [0 diff(data(2,:))];
timeVector(timeVector < -60000) = timeVector(timeVector<-60000) + 65536;
timeVector = cumsum(timeVector)./1000;
voltageVector = 5.*data(3,:)./1024;
voltageDelta = [0 diff(voltageVector)];

% Plot full trace + zoom in + difference
figure('color','w')
subplot(2,1,1),plot(timeVector,voltageVector)
subplot(2,1,2),hold on
plot(timeVector(16000:20500),voltageVector(16000:20500))
plot(timeVector(16000:20500),voltageDelta(16000:20500),'r')

xlabel('Time [ms]')
ylabel('Voltage [v]')

% Plot distributions of tags sent from stimulus pc
figure('color','w')
tagTimes = timeVector(tagPositions);
tagTimesDelta = diff(tagTimes);
x = rand(1,length(tagTimesDelta));
subplot(1,2,1),plot(x,tagTimesDelta,'.')
ylabel('Delta T [ms]')
title('Time between tag signals')

%% Analyse photocell trace
% Set an absolute threshold and calculate time between absolute threshold
% crossings
threshold = 1.5;


onsetIndices = [];
firstPass = 0;
for i = 1:length(voltageVector)
    if voltageVector(i) > threshold && firstPass == 0 && voltageDelta(i-1) > 0
        onsetIndices = [onsetIndices i];
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
