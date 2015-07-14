classdef RewardDelivery < handle
    % RewardDelivery: Class that handles the interface with the Measurement
    % Computing toolbox
    % Important note: If Matlab or drivers are reinstalled, use
    % daqregister('mcc') after the installation before running this class
    
    properties
        boardNumber = 0;
        relayChannels = 0;
        boardHandle = 0;
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
            catch err
                display('Could not connect to measurement computing board: ');
                display(err.message);
            end
        end
        function delete(obj)
            obj.StopReward();
        end

        % Handles reward delivery
        function StartReward(obj)
            putvalue(obj.boardHandle, ones(1,length(obj.relayChannels)));
        end
        
        function StopReward(obj)
            putvalue(obj.boardHandle, zeros(1,length(obj.relayChannels)));
        end
    end
    
end

