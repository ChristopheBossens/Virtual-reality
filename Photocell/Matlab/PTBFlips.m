% Analyse the screen flip data output from psychophysics toolbox
% The data is structured as follows:
% 1. VBLTimestamp          Estimate of when actual flip has happened [s]
% 2. StimulusOnsetTime     Estimate of stimulus onset time [s]
% 3. FlipTimestamp         End of flip excecution [s]
% 4. Missed                Negative values means deadline has been met
% 5. Beampos
% 6. Start flip measured by tic...toc mechanism
% 7. Stop flip measured by tic...toc mechanism
data = load('F:\Projects\VirtualReality\Data\screenRightStereo4.mat');

screenVBL = data.dataMatrix(:,1);
screenSOT = data.dataMatrix(:,2);
screenFTS = data.dataMatrix(:,3);
screenMIS = data.dataMatrix(:,4);
screenPOS = data.dataMatrix(:,5);
screenFlipStart = data.dataMatrix(:,6)
screenFlipStop = data.dataMatrix(:,7);

%% Analyse and present the data
nFrames = size(screenVBL,1)
screenMisses = length(find(screenMIS> 0)); % [ms]
screenFlipInterval = 1000*diff(screenVBL); % [ms]
screenFlipExcecution = 1000*(screenFTS-screenVBL); % [ms]
screenFlipIntervalFromTic = 1000.*(screenFlipStart(2:end)-screenFlipStart(1:(end-1))); % [ms]
screenFlipExcecutionFromTic = 1000.*(screenFlipStop-screenFlipStart);   % ]ms]



x = linspace(0,1,length(screenFlipInterval));
figure('color','white','name',['Frame diagnostics @ ' num2str(nFrames) 'frames'])
subplot(2,2,1)
plot(x,screenFlipInterval,'.'), hold on
plot(x + 2, screenFlipIntervalFromTic,'.')
set(gca,'XLim',[-1 4],'XTick',[0.5 2.5],'XTickLabel',{'PTB' 'From tic-toc'})
ylabel('Time between flips [ms]')
title('Flip interval distribution')

subplot(2,2,2)
x = rand(1,length(screenFlipExcecution));
plot(x,screenFlipExcecution,'.'),hold on
plot(x + 2,screenFlipExcecutionFromTic,'.'),
set(gca,'XLim',[-1 4],'XTick',[0.5 2.5],'XTickLabel',{'PTB' 'From tic-toc'})
ylabel('Flip excecution time [ms]')
title('Flip excecution time')

subplot(2,2,3)
x = rand(1,length(screenPOS))
plot(x,screenPOS,'.'),hold on
%set(gca,'XLim',[-1 4],'XTick',[0.5 2.5],'XTickLabel',{'Left screen' 'Right screen'})
ylabel('Beam position [ms]')
title('Beam position @ flip')

subplot(2,2,4)
bar([0.5],[screenMisses])
set(gca,'XLim',[0 1],'XTick',[0.5],'XTickLabel',{''},'YLim',[0 screenMisses+1])
ylabel('Misses [count]')
title('Screen flip misses')