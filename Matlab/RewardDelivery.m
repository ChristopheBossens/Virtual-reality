classdef RewardDelivery < handle
    % RewardDelivery: Class that handles the interface with the Measurement
    % Computing toolbox
    % Important note: If Matlab or drivers are reinstalled, use
    % daqregister('mcc') after the installation before running this class
    
    properties
        boardNumber = 0;
        relayChannels = 0;
        boardHandle = 0;
        isConnected = 0;
    end
    
    methods
        % Class constructor and destructors
        function obj = RewardDelivery(boardNumber, relayChannels)
            try
                obj.boardNumber = boardNumber;
                obj.relayChannels = relayChannels;
                obj.boardHandle = digitalio('mcc',boardNumber);
                addline(obj.boardHandle,relayChannels,'out');
                display('Connected to relay board');
                obj.isConnected = 1;
            catch err
                display('Could not connect to relay board. No rewards will be delivered.');
                display(['Error message: ' err.message]);
                obj.isConnected = 0;
            end
        end
        
        function delete(obj)
            if (obj.isConnected == 1)
                obj.StopReward();
                delete(obj.boardHandle);
                display('Disconnected from relay board');
            end
        end

        % Handles reward delivery
        function StartReward(obj)
            if (obj.isConnected == 1)
                putvalue(obj.boardHandle, ones(1,length(obj.relayChannels)));
            end
        end
        
        function StopReward(obj)
            if (obj.isConnected == 1)
                putvalue(obj.boardHandle, zeros(1,length(obj.relayChannels)));
            end
        end
    end
    
end

