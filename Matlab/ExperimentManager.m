classdef ExperimentManager < handle
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        frameParameters = [];
        frameData = [];
        
        motionSensor = [];
        motionSensorPort = 'COM8';
        LD = 31;
        CPI = 9;
        
        rewardDelivery = []
        rewardBoardNumber = 0;          % Measurement Computing relay board
        rewardLines = 1;                % Selects the relay port
        maxRewardFrames = 3;             % Specify number of frames the relay is opened
        clearColor = [0.2 0.2 0.2];
        
        fullscreenWindow = 0;
        fullscreenRect = 0; 
        
        imageData = {};
        trialData = {};
        imageLUT = [];
        
        logfile = '';
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
            obj.Close();
        end

        function InitScreen(obj, whichScreen)
            % Initialize PTB screens, screen number can be given as an
            % extra argument
            [obj.fullscreenWindow, obj.fullscreenRect] = PsychImaging('OpenWindow',whichScreen,[],[],[],4);
            Screen('FillRect',obj.fullscreenWindow,obj.clearColor,obj.fullscreenRect);
            Screen('Flip',obj.fullscreenWindow);
        end
        function InitMotionSensor(obj, portname, LD, CPI)
            obj.motionSensorPort = portname;
            obj.motionSensor.LD = LD;
            obj.motionSensor.CPI = CPI;
            
            obj.motionSensor = MotionSensor(obj.motionSensorPort);
            obj.motionSensor.SetParams([obj.CPI obj.CPI obj.LD obj.LD]);
            
            CPI = obj.motionSensor.GetCPI();
            LD = obj.motionSensor.GetLD();
            if numel(LD) < 2
                display('No valid parameters found');
            else
                display('Using Motion sensor parameters:');
                display(['LD: ' num2str(LD(1)) ', ' num2str(LD(2))]);
                display(['DPI: ' num2str(CPI(1)) ', ' num2str(CPI(2))]);
            end
        end           
        function InitRewardDelivery(obj, boardNumber, rewardLine)
            obj.rewardBoardNumber = boardNumber;
            obj.rewardLines = rewardLine-1;
            obj.rewardDelivery = RewardDelivery(obj.rewardBoardNumber,obj.rewardLines);
            display('Reward delivery relay initialized');
        end
        
        function [screenID, screenRect] = GetScreenInformation(obj)
            screenID = obj.fullscreenWindow;
            screenRect = obj.fullscreenRect;
        end
        function Close(obj)
            delete(obj.rewardDelivery);
            delete(obj.motionSensor);
            Screen('CloseAll');
        end
        
        function PulseReward(obj)
            obj.rewardDelivery.StartReward();
            obj.rewardDelivery.StopReward();
        end
        
        function status = LoadXML(obj,file)
            % Loads an experimental xml file
            % A check is conducted to make sure that all stimuli mentioned
            % in across trials are also present in the stimulus list
            status = 1;
            try
                [obj.imageData, obj.trialData] = xmlParser(file);
                display('XML parsed without problems, checking internal consistency.');
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
                    status = 0;
                end
            catch exception
                display(exception)
                status = 0;
            end
            display('Internal consistency ok. Textures can be loaded');
        end
        
        function status = PrepareTextures(obj)
            % Puts the stimulus list extracted from the experimental XML
            % file onto the video card
            status = 1;
            try
                % Clear any existing textures if they exist
                if ~isempty(obj.imageLUT)
                   for imageIndex = 1:size(obj.imageLUT,1)
                       Screen('Close',obj.imageLUT(:,2));
                   end
                end
                
                % Upload current experiment stimuli
                imageCount = size(obj.imageData,1);
                obj.imageLUT = zeros(imageCount,2);
                for imageIndex = 1:imageCount
                    stimulus = imread(obj.imageData{imageIndex,3});
                    textureID = Screen('MakeTexture',obj.fullscreenWindow,stimulus);

                    obj.imageLUT(imageIndex,1) = obj.imageData{imageIndex,1};
                    obj.imageLUT(imageIndex,2) = textureID;
                end
                display(['Succesfully loaded ' num2str(imageCount) ' images on GPU']);
            catch exception
                display(exception)
                status = 0;
            end
            
        end
        
        function RunExperiment(obj, logfile, client)
            % Prepares all the devices and then presents each trial
            obj.logfile = fopen(logfile,'w+');
            obj.motionSensor.StartReading();
            nTrials = size(obj.trialData,1);
            
            display('Running experiment...');
            for trialIndex = 1:nTrials
                % signal current trial to vrClient
                if nargin == 3
                    fwrite(client,[7 2],'uint8');
                    fwrite(client,[trialIndex nTrials],'uint16');
                end
                
                % Run trial
                obj.RunTrial(trialIndex);
            end
            
            obj.motionSensor.StopReading();
            fclose(obj.logfile);
            
            Screen('FillRect',obj.fullscreenWindow,obj.clearColor,obj.fullscreenRect);
            Screen('Flip', obj.fullscreenWindow);
            
            display('Experiment Finished!');
        end
        
        function RunTrial(obj, trialIndex)
            % Load parameters for the current trial and start trial timer
            trialFinished=0;
            tic;
            lastRead = toc;           
            frameCount = 0;
            
            % Load reward type
            rewardActivated = false;
            rewardFrames = 0;
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
            
            % Load transition type
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
            
            % Prepare stimulus parameters
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
            
            % Write trial header to data file
            fwrite(obj.logfile,[255 255 255 255],'uint8');
            fwrite(obj.logfile,trialIndex,'uint16');
            
            % Run the trial until transition condition is met
            while(trialFinished==0)
                % Get velocity
                [x,y] = obj.motionSensor.ReadSensor();
                currentTimestamp = toc;
                timeInterval = currentTimestamp - lastRead;
                lastRead = currentTimestamp;
                
                xVelocity = abs(x/timeInterval);
                yVelocity = abs(y/timeInterval);
                % Check reward
                if (rewardType == obj.REWARD_VELOCITY)
                    if (rewardComparison == obj.HIGHER) && (xVelocity > rewardThreshold)
                        rewardThresholdCount = rewardThresholdCount + 1;
                    elseif (rewardComparison == obj.LOWER) && (xVelocity < rewardThreshold)
                        rewardThresholdCount = rewardThresholdCount + 1;
                    else
                        rewardThresholdCount = 0;
                    end
                    
                    % Velocity criterion met
                    if (rewardThresholdCount >= rewardDuration)
                        rewardActivated = true;
                        rewardFrames = 0;
                        obj.rewardDelivery.StartReward();
                        rewardThresholdCount = 0;
                    end
                elseif (rewardType == obj.REWARD_RANDOM)
                    % Random criterion met
                    if frameCount == rewardStart
                        rewardFrames = 0;
                        rewardActivated = true;
                        obj.rewardDelivery.StartReward();
                    end
                end
                
                if rewardActivated == true
                    rewardFrames = rewardFrames + 1;
                    if rewardFrames == obj.maxRewardFrames
                        rewardFrames = 0;
                        rewardActivated = false;
                        try
                            obj.rewardDelivery.StopReward();
                            obj.rewardDelivery.StopReward();
                        catch ME
                        end
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
                        obj.rewardDelivery.StopReward();
                        return;
                    end
                elseif (transitionType == obj.TRANSITION_DURATION)
                    if (frameCount > transitionDuration)
                        obj.rewardDelivery.StopReward();
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
                
                % Write logfile data
                fwrite(obj.logfile,[currentTimestamp xVelocity yVelocity],'float');
                fwrite(obj.logfile,frameCount,'uint16');
                fwrite(obj.logfile,rewardActivated,'uint8');
            end
        end      
        
    end
end

