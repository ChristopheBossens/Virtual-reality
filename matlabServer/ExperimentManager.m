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
        
        fullscreenWindow = 0;
        fullscreenRect = 0; 
        
        imageData = {};
        trialData = {};
        imageLUT = [];
    end
    
    properties(Constant)
        REWARD_NONE = 1;
        REWARD_RANDOM = 2;
        REWARD_VELOCITY = 3;
        
        TRANSITION_VELOCITY = 1;
        TRANSITION_DURATION = 2;
        
        HIGHER = 1;
        LOWER = 2;
    end
    
    methods  
        % Class constructors and destructors
        function obj = ExperimentManager()
            
        end
        function delete(obj)
            Screen('CloseAll');
        end

        function InitScreen(obj, whichScreen)
            [obj.fullscreenWindow, obj.fullscreenRect] = PsychImaging('OpenWindow',whichScreen,[],[],[],4);
            
        end
        
        function InitPeripherals(obj)
            obj.motionSensor = MotionSensor(obj.motionSensorPort);
            obj.rewardDelivery = RewardDelivery(obj.rewardBoardNumber,obj.rewardLines);
        end
               
        function LoadXML(obj,file)
            try
                [obj.imageData, obj.trialData] = xmlParser(file);
                
                % Check that all stimulus names are present in the image
                mismatch = [];
                for trialIndex = 1:size(obj.trialData,1)
                    leftStimulusName = obj.trialData{trialIndex,16};
                    rightStimulusName = obj.trialData{trialIndex,24};
                    leftStimulusID = strcmp(leftStimulusName,obj.imageData(:,2));
                    rightStimulusID =strcmp(rightStimulusName,obj.imageData(:,2));
                    
                    if (sum(leftStimulusID) ~= 1 || sum(rightStimulusID) ~= 1)
                        mismatch = [mismatch trialIndex];
                    end
                end
                
                if ~isempty(mismatch)
                    display('Error, the following trials do not have a matching name in the image list:')
                    display(mismatch)
                end
            catch exception
                display(exception)
            end
        end
        
        function PrepareTextures(obj)
            try
                % Clear any existing textures if they exist
                if ~isempty(obj.imageLUT)
                   for imageIndex = 1:size(obj.imageLUT,1)
                       Screen('Close',obj.imageLUT(:,2));
                   end
                end
                
                
                imageCount = size(obj.imageData,1);
                obj.imageLUT = zeros(imageCount,2);
                for imageIndex = 1:imageCount
                    stimulus = imread(obj.imageData{imageIndex,3});
                    textureID = Screen('MakeTexture',obj.fullscreenWindow,stimulus);

                    obj.imageLUT(imageIndex,1) = obj.imageData{imageIndex,1};
                    obj.imageLUT(imageIndex,2) = textureID;
                end
            catch exception
                display(exception)
            end
        end
        
        function RunExperiment(obj)
            
            for trialIndex = 1:size(obj.trialData,1)
                % signal current trial to vrClient
                trialName = obj.trialData{trialIndex,2};
                display(trialName);
                
                % Run trial
                obj.RunTrial(trialIndex);
            end
        end
        
        function RunTrial(obj, trialIndex)
            % Load parameters for the current trial
            trialFinished=0;
            tic;
            lastRead = toc;
            
            frameCount = 0;
            
            switch obj.trialData{trialIndex,3}
                case 'none'
                    rewardType = obj.REWARD_NONE;
                case 'random'
                    rewardType = obj.REWARD_RANDOM;
                    rewardStart = obj.trialData{trialIndex,4};
                case 'velocity'
                    rewardType = obj.REWARD_VELOCITY;
                    rewardThreshold = obj.trialData{trialIndex,4};
                    rewardDuration = obj.trialData{trialIndex,5};
                    if strcmp(obj.trialData{trialIndex,6},'higher')
                        rewardComparison = obj.HIGHER;
                    else
                        rewardComparison = obj.LOWER;
                    end
                    rewardThresholdCount = 0;
            end
            
            switch obj.trialData{trialIndex,9}
                case 'velocity'
                    transitionType = obj.TRANSITION_VELOCITY;
                    transitionThreshold = obj.trialData{trialIndex,10};
                    transitionDuration = obj.trialData{trialIndex,11};
                    if strcmp(obj.trialData{trialIndex,12},'higher')
                        transitionComparison = obj.HIGHER;
                    else
                        transitionComparison = obj.LOWER;
                    end
                    transitionThresholdCount = 0;
                case 'duration'
                    transitionType = obj.TRANSITION_DURATION;
                    transitionDuration = obj.trialData{trialIndex,10};
            end
            
            leftStimulusName = obj.trialData{trialIndex,16};
            rightStimulusName = obj.trialData{trialIndex,24};
            leftStimulusID = find(strcmp(leftStimulusName,obj.imageData(:,2)));
            rightStimulusID =find(strcmp(rightStimulusName,obj.imageData(:,2)));
            leftStimulusTex = obj.imageLUT(leftStimulusID(1),2);
            rightStimulusTex =obj.imageLUT(rightStimulusID(1),2);
            leftStimulusPos  = [obj.trialData{trialIndex,17:20}];
            rightStimulusPos  = [obj.trialData{trialIndex,25:28}];
            
            leftStimulusSource= [0 0 leftStimulusPos(1:2)];
            leftStimulusDestination = [leftStimulusPos(3:4) leftStimulusPos(1:2)+leftStimulusPos(3:4)];
            
            rightStimulusSource= [0 0 rightStimulusPos(1:2)];
            rightStimulusDestination = [rightStimulusPos(3:4) rightStimulusPos(1:2)+rightStimulusPos(3:4)];
            rightStimulusDestination(1) = rightStimulusDestination(1) + obj.fullscreenRect(3)/2;
            rightStimulusDestination(3) = rightStimulusDestination(3) + obj.fullscreenRect(3)/2;
            % Run the trial until the condition to finish is met
            while(trialFinished==0)
                % Get velocity
                [x,y] = obj.motionSensor.ReadSensor();
                timeInterval = toc - lastRead;
                lastRead = tic;
                xVelocity = abs(x/timeInterval);
                
                % Check reward
                if (rewardType == obj.REWARD_VELOCITY)
                    if (rewardComparison == obj.HIGHER) && (xVelocity > rewardThreshold)
                        rewardThresholdCount = rewardThresholdCount + 1;
                    elseif (rewardComparison == obj.LOWER) && (xVelocity < rewardThreshold)
                        rewardThresholdCount = rewardThresholdCount + 1;
                    else
                        rewardThresholdCount = 0;
                    end
                    
                    if (rewardThresholdCount >= rewardDuration)
                        % Give reward
                        display('Threshold reward delivered');
                        rewardThresholdCount = 0;
                    end
                elseif (rewardType == obj.REWARD_RANDOM)
                    if frameCount == rewardStart
                        display('Random reward delivered');
                        % Give reward
                    end
                end
                
                % Check transition
                if (transitionType == obj.TRANSITION_VELOCITY)
                    if (transitionComparison == obj.HIGHER) && (xVelocity > transitionThreshold)
                        transitionThresholdCount = transitionThresholdCount + 1;
                    elseif (transitionComparison == obj.LOWER) && (xVelocity < transitionThreshold)
                        transitionThresholdCount = transitionThresholdCount + 1;
                    else
                        transitionThresholdCount = 0;
                    end
                    
                    if (transitionThresholdCount >= transitionDuration)
                        display('Transition on velocity');
                        return;
                    end
                elseif (transitionType == obj.TRANSITION_DURATION)
                    if (frameCount > transitionDuration)
                        display('Transition on framecount');
                        return;
                    end
                end
                
                % Update stimulus
                stimulusSource = [leftStimulusSource' rightStimulusSource'];
                stimulusDestination = [leftStimulusDestination' rightStimulusDestination'];
                
                % Draw stimulus
                Screen('DrawTextures',obj.fullscreenWindow, [leftStimulusTex rightStimulusTex],stimulusSource,stimulusDestination);
                [VLB SO FT M B]  = Screen('Flip',obj.fullscreenWindow);
                frameCount = frameCount + 1;
            end
        end      
        
    end
end
