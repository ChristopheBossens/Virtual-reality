function GetDisplayInfo()
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

screenNumbers = Screen('Screens');

for i = 1:length(screenNumbers)
    screenIndex = screenNumbers(i);
   frameRate = Screen('FrameRate', screenIndex);
   nominalFrameRate = Screen('NominalFrameRate',screenIndex);

   display(['Screen '  num2str(screenIndex)  ': ']);
   display(['frame rate: ' num2str(frameRate)]);
   display(['nominal framerate: ' num2str(nominalFrameRate)]);  
   
end

end

