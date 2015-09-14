% Analyse the screen flip data output from psychophysics toolbox
% The data is structured as follows:
% 1. VBLTimestamp          Estimate of when actual flip has happened [s]
% 2. StimulusOnsetTime     Estimate of stimulus onset time [s]
% 3. FlipTimestamp         End of flip excecution [s]
% 4. Missed                Negative values means deadline has been met
% 5. Beampos
% And 6...10 contains the same information for the right screen
data = xlsread('C:\Users\Christophe\Documents\GitHub\Virtual-reality\Photocell\Calibration\frameData.xlsx');

screen1VBL = data(:,1)
screen1SOT = data(:,2)
screen1FTS = data(:,3)
screen1MIS = data(:,4)
screen1POS = data(:,5)

screen2VBL = data(:,6)
screen2SOT = data(:,7)
screen2FTS = data(:,8)
screen2MIS = data(:,9)
screen2POS = data(:,10)
%% Data analysis
nFrames = size(data,1)
screen1Misses = length(find(screen1MIS> 0));
screen2Misses = length(find(screen2MIS> 0));

screen1FlipInterval = 1000*diff(screen1VBL);
screen2FlipInterval = 1000*diff(screen2VBL);

screen1FlipExcecution = 1000*(screen1FTS-screen1VBL);
screen2FlipExcecution = 1000*(screen2FTS-screen2VBL);

screenFlipDiff = 1000*(screen1FTS-screen2FTS);

x = rand(1,length(screen1FlipInterval));
figure('color','white','name',['Frame diagnostics @ ' num2str(nFrames) 'frames'])
subplot(2,3,1)
plot(x,screen1FlipInterval,'.'), hold on
plot(x + 2, screen2FlipInterval,'.')
set(gca,'XLim',[-1 4],'XTick',[0.5 2.5],'XTickLabel',{'Left screen' 'Right screen'})
ylabel('Time between flips [ms]')
title('Flip interval distribution')

subplot(2,3,2)
x = rand(1,length(screen1FlipExcecution));
plot(x,screen1FlipExcecution,'.'),hold on
plot(x + 2,screen2FlipExcecution,'.'),
set(gca,'XLim',[-1 4],'XTick',[0.5 2.5],'XTickLabel',{'Left screen' 'Right screen'})
ylabel('Flip excecution time [ms]')
title('Flip excecution time')

subplot(2,3,3)
x = rand(1,length(screen2POS))
plot(x,screen1POS,'.'),hold on
plot(x + 2,screen2POS,'.'),
set(gca,'XLim',[-1 4],'XTick',[0.5 2.5],'XTickLabel',{'Left screen' 'Right screen'})
ylabel('Beam position [ms]')
title('Beam position @ flip')

subplot(2,3,4)
bar([0.5 2.5],[screen1Misses screen2Misses])
set(gca,'XLim',[-1 4],'XTick',[0.5 2.5],'XTickLabel',{'Left screen' 'Right screen'})
ylabel('Misses [count]')
title('Screen flip misses')

subplot(2,3,5)
plot(x,screenFlipDiff,'.')
ylabel('Beam position [ms]')
title('Time difference between left and righ screen flip')
