%% analyseNlxEvents.m
% Used to analyse timing information provided by the neuralynx system. Data
% is send over TTL ports using the Measurement Computing DAQ system and
% with a photocell event detector.
filename = 'E:\Data\Running_ball_V1_Adaptation\Data\2016-04-06_16-40-56\Events.nev';
fieldSelection = [1 1 1 0 0];
extractHeader = 0;
extractionMode = 1;
modeArray = 1;
[Timestamps, EventIDs, TTLs, Extras, EventStrings, Header] = Nlx2MatEV(filename, [1 1 1 1 1], 1, 1, [] );

% Cell array string search funcinos
substrmatch = @(x,y) ~cellfun(@isempty,strfind(y,x));

% Find data indices for a specific animal
animalID = '68681r0';
startIdx = find(substrmatch(['start experiment ' animalID], EventStrings)==1);
stopIdx  = find(substrmatch(['stop experiment ' animalID], EventStrings)==1);

% portZero corresponds to DAQ signals, portOne corresponds to photocell
% event signals
portZeroTTL = substrmatch('port 0', EventStrings);
portOneTTL = substrmatch('port 1', EventStrings);
%% Get a histogram for the condition numbers.
% Can be used to validate that the conditions follow the distribution you
% had in mind. Furthermore, timing between condition events can be used to
% probe the timing between conditions.

conditionIdx = find(portZeroTTL == 1);
conditionIdx = conditionIdx(conditionIdx > (startIdx+50) & conditionIdx < (stopIdx-50));

conditions = unique(TTLs(conditionIdx));
counts = zeros(1,length(conditions));
for i = 1:length(conditions)
    counts(i) = sum(TTLs(conditionIdx) == conditions(i));
end

% Inspect deltas between condition information
conditionDeltas = diff(Timestamps(conditionIdx))./1000;
edges = [650:1:750];
edgeCount = hist(conditionDeltas(conditionDeltas < 800),edges);
clf,
subplot(2,1,1),bar(conditions,counts),
title('Condition code histogram')
xlabel('Condition number')
ylabel('Frequency')
subplot(2,2,3),
hold on
plot([700-8, 700+8],[0 0], 'vr','MarkerFaceColor','r')
bar(edges, edgeCount),
legend('Valid frame interval')
title('Time between trial onset/offset'),xlabel('Time (ms)'),ylabel('Frequency')

edges = [1450:1:1550];
edgeCount = hist(conditionDeltas(conditionDeltas > 800),edges);
subplot(2,2,4),
hold on
plot([1500-8, 1500+8],[0 0], 'vr','MarkerFaceColor','r')
bar(edges, edgeCount),
legend('Valid frame interval')
title('Time between trial offset/onset'),xlabel('Time (ms)'),ylabel('Frequency')
%% Inspect photocell events
% First we filter the photocell events: only events with a minimum of
% separation of 16 ms will be counted. That way we eliminate spurious
% photocell event data caused by transition noise. From the distribution of
% photocell event data we see that the actual time between an on and
% off-event is centered at 27 ms so our cutoff is definitely conservative
% enough
photoIdx = find(portOneTTL==1);
photoIdx = photoIdx(photoIdx > startIdx & photoIdx < stopIdx);

validPhotoIdx = zeros(1,length(photoIdx));
nValid = 1;
currentIdx = 1;
nextIdx = 2;
validPhotoIdx(currentIdx) = photoIdx(currentIdx);
while nextIdx <= length(photoIdx)
    if (Timestamps(photoIdx(nextIdx))-Timestamps(photoIdx(currentIdx)))/1000 > 16
        nValid = nValid + 1;
        validPhotoIdx(nValid) = photoIdx(nextIdx);
        currentIdx = nextIdx;
    end
    nextIdx = nextIdx + 1;
end
validPhotoIdx = validPhotoIdx(1:nValid);

% With the correct indices we can now construct photocell event
% distributions
photocellDeltas = diff(Timestamps(validPhotoIdx))./1000;
photocellDeltas = photocellDeltas(10: (length(photocellDeltas-10)));

edges = [0:1:500];
edgeCount = hist(photocellDeltas,edges);

clf,
bar(edges,edgeCount)
title('Timing between photocell events')
xlabel('Time (ms)')
ylabel('Frequency')
%% Relation between DAQ events and photocell events
% If we receive a condition event, we now that the next event will always
% be a photocell on event. We only need to inspect the time difference
% between each event.
conditionIdx = find(portZeroTTL == 1);
conditionIdx = conditionIdx(conditionIdx > startIdx & conditionIdx < stopIdx);

photoIdx = conditionIdx + 1;
eventDelta = ((Timestamps(photoIdx)-Timestamps(conditionIdx))/1000);
edges=0:0.1:100;
edgeCount = hist(eventDelta,edges);
clf,
bar(edges,edgeCount);
xlabel('Time (ms)')
ylabel('Frequency')
title('Time between condition information and photocell event')
