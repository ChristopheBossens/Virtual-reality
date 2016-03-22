classdef Transition < handle
    properties
        type;
        
        thresholdVelocity;
        thresholdDuration;
        thresholdComparator;
        
        trialDuration;
    end
    
    methods
        function obj = Transition(type)
            if nargin ~= 1
                error('Insuffucient parameters.')
            end
            
            switch type
                case 'velocity'
                    obj.type = type;
                    obj.SetVelocityParams(5.0,60,'>');
                case 'duration'
                    obj.type = type;
                    obj.SetTrialDuration(60);
                otherwise
                    error('Transition type not recognized');
            end
        end
        
        % Set parameters for velocity threshold
        %  thresholdVelocity: expressed in cm/s
        %  thresholdDuration: number of frames above or below threshold
        %  thresholdComparator: determines if we track if velocity is above
        %  threshold or below threshold
        % Example SetVelocityParams(5.0, 60, '>') stops the trial when
        % velocity is above 5.0 cm/s for 60 frames
        function SetVelocityParams(obj, thresholdVelocity,thresholdDuration,thresholdComparator)
            if nargin ~= 4
                error('Incorrect number of parameters')
            end
            
            if strcmp(obj.type,'velocity')
                if strcmp(thresholdComparator,'<') == 1 || strcmp(thresholdComparator,'>') == 1
                    obj.thresholdVelocity = thresholdVelocity;
                    obj.thresholdDuration = thresholdDuration;
                    obj.thresholdComparator = thresholdComparator;
                else
                    error('comparator should equal ''<'' or ''>'' ');
                end
            else
                error('Reward is not of velocity type');
            end
        end
        
        % Set parameters for interval reward
        %  intervalDuration: how many frames to wait before giving a reward
        %  repeat: 'on' or 'off' if we want to give repeated rewards at
        %  that interval
        function SetTrialDuration(obj, trialDuration)
            if nargin ~= 2
                error('Incorrect number of parameters')
            end
            
            if strcmp(obj.type, 'duration')
                    obj.trialDuration = trialDuration;
            else
                error('Reward is not of interval type');
            end
        end
    end
end