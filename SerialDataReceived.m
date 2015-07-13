function SerialDataReceived( obj, event, plotHandle )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
display(['Callback triggered'])
    global packetsReceived
    global RX;
    RX = 1;
    yValues = get(plotHandle, 'YData');
    
    while obj.BytesAvailable > 20 && RX == 1
        newY = fread(obj,10,'int16')
        yValues = [newY' yValues(11:end)];
        packetsReceived = packetsReceived + 1;
        
        if (mod(packetsReceived,100) == 0)
            set(plotHandle, 'YData', yValues);
            drawnow()
        end
        pause(0.01);
    end
end

