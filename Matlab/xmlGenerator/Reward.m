classdef Reward < handle
    properties
        type;
        thresholdVelocity;
        thresholdDuration;
        thresholdComparator;
        
        intervalDuration;
        repeatInterval;
    end
    
    methods
        function obj = Reward(type)
            if nargin ~= 1
                error('Insufficient parameters.');
            end
            
            switch type
                case 'velocity'
                    obj.type = type;
                    obj.SetVelocityParams(5.0,60,'>');
                case 'interval'
                    obj.type = type;
                    obj.SetIntervalParams(60,'off');
                case 'none'
                    obj.type = type;
                otherwise
                    error('Incorrect parameter value');
            end
        end
        
        % Set parameters for velocity threshold
        %  thresholdVelocity: expressed in cm/s
        %  thresholdDuration: number of frames above or below threshold
        %  thresholdComparator: determines if we track if velocity is above
        %  threshold or below threshold
        % Example SetVelocityParams(5.0, 60, '>') delivers a reward if
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
        function SetIntervalParams(obj, intervalDuration, repeatInterval)
            if nargin ~= 3
                error('Incorrect number of parameters')
            end
            
            if strcmp(obj.type, 'interval')
                if strcmp(repeatInterval,'on') == 1 || strcmp(repeatInterval,'off') == 1
                    obj.intervalDuration = intervalDuration;
                    obj.repeatInterval = repeatInterval;
                else
                    error('repeatInterval should be ''on'' or ''off'' ');
                end
            else
                error('Reward is not of interval type');
            end
        end
    end
end