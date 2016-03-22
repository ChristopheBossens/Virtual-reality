classdef Trial < handle
    properties
        order;
        condition;
        
        rewardObject;
        transitionObject;
        stimulusList;
    end
    
    methods
        % Creates a default trial with no rewards and transition after 1
        % second
        function obj = Trial(rewardObject,transitionObject,stimulusObject)
            if nargin == 0
                obj.transitionObject = Transition('duration');
                obj.rewardObject = Reward('none');
            elseif nargin == 3
                obj.transitionObject = transitionObject;
                obj.rewardObject = rewardObject;
                obj.stimulusList = stimulusObject;
            end
            obj.condition = 1;
        end
        
        function SetOrder(obj, order)
            obj.order = order;
        end
        
        function SetCondition(obj,condition)
            obj.condition = condition;
        end
        
        function SetReward(obj,rewardObject)
            obj.rewardObject = rewardObject;
        end
        
        function SetTransition(obj,transitionObject)
            obj.transitionObject= transitionObject;
        end
        
        function AddStimulus(obj, stimulusObject)
            obj.stimulusList = [obj.stimulusList;stimulusObject];
        end
        
        function SetStimulus(obj, stimulusObject)
            obj.stimulusList = [stimulusObject];
        end
    end
end