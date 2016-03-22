classdef TrialBlock < handle
    properties
        trialList;
        shuffleOrder;
    end
    
    methods
        function obj = TrialBlock(shuffleOrder)
            if nargin == 0
                shuffleOrder = 'random';
            end
            
            if ~(strcmp(shuffleOrder,'random') || strcmp(shuffleOrder,'fixed'))
                error('Shuffle order not recognized');
            end
            
            obj.shuffleOrder = shuffleOrder;
        end
        
        function AddTrial(obj,trialObject)
            switch obj.shuffleOrder
                case 'random'
                    
                case 'fixed'
                    trialObject.SetOrder(length(obj.trialList)+1);
            end
            obj.trialList = [obj.trialList;trialObject];
        end
    end
end
