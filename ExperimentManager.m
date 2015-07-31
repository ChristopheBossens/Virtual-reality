classdef ExperimentManager < handle
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        frameParameters = [];
        frameData = [];
        
        motionSensor = [];
        motionSensorPort = 'COM8';
        
        rewardDelivery = []
        rewardBoardNumber = 0;
        rewardLines = 1;
        
        clearColor = [0.2 0.2 0.2];
        lMonitorIndex = 2;
        rMonitorIndex = 3;
        
        lMonitorWindow = 0;
        rMonitorWindow = 0;
        lMonitorRect = 0;
        rMonitorRect = 0;
        
        % UDP client for screen testing purposes only
        destPort = 6789;
        localPort = 6788;
    end
    
    methods  
        % Class constructors and destructors
        function obj = ExperimentManager()
            
        end
        function delete(obj)
        end

        function Init(obj)
            [obj.lMonitorWindow, obj.lMonitorRect] = Screen('OpenWindow',obj.lMonitorIndex, obj.clearColor);
            [obj.rMonitorWindow, obj.rMonitorRect] = Screen('OpenWindow',obj.rMonitorIndex, obj.clearColor);
            obj.motionSensor = MotionSensor(obj.motionSensorPort);
            obj.rewardDelivery = RewardDelivery(obj.rewardBoardNumber,obj.rewardLines);
        end
        
        function AttachFrames(obj,frameParameters)
            obj.frameParameters = frameParameters;
        end
        
        % To do: add method to detect stimulus change so that we can
        % illuminate the photosensor for stimulus onset and stimulus offset
        function RunFrames(obj)
            nFrames = size(obj.frameParameters,1);
            obj.frameData = zeros(nFrames,20);
            
            obj.motionSensor.StartReading();
            client = udp('127.0.0.1',obj.destPort);
            fopen(client)
            for frameIndex = 1:nFrames
                % Readvelocity
                [x,y] = obj.motionSensor.ReadSensor();
                
                % Update framebuffer
                if ( obj.frameParameters(frameIndex,1) == 0)
                    Screen('FillRect',obj.lMonitorWindow,obj.clearColor);
                else
                    Screen('DrawTexture', obj.lMonitorWindow, obj.frameParameters(frameIndex,1),[],[],0);
                end

                if ( obj.frameParameters(frameIndex,2) == 0)
                    Screen('FillRect',obj.rMonitorWindow,obj.clearColor);
                else
                    Screen('DrawTexture', obj.rMonitorWindow, obj.frameParameters(frameIndex,2),[],[],0);
                end

                % Swap buffer and collect data
                fwrite(client,20)
                [VLB1 SO1 FT1 M1 B1]  = Screen('Flip',obj.lMonitorWindow);
                [VLB2 SO2 FT2 M2 B2] = Screen('Flip',obj.rMonitorWindow);
                obj.frameData(frameIndex,1:10) = [VLB1 SO1 FT1 M1 B1 VLB2 SO2 FT2 M2 B2];
            end
            fclose(client)
            obj.motionSensor.StopReading();

            Screen('FillRect',obj.lMonitorWindow, obj.clearColor);
            Screen('FillRect',obj.rMonitorWindow, obj.clearColor);
            Screen('Flip',obj.lMonitorWindow);
            Screen('Flip',obj.rMonitorWindow);
        end
    end
end

