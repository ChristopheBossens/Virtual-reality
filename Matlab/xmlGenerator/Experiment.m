classdef Experiment < handle
    properties
        blockList;
        uniqueStimulusList;
    end
    
    methods
        function obj = Experiment()
        end
        
        function AddBlock(obj, blockObject)
            obj.blockList = [obj.blockList; blockObject];
        end
        
        function WriteXML(obj)
            if isempty(obj.blockList)
                error('No experimental blocks found.');
            end
            
            docNode = com.mathworks.xml.XMLUtils.createDocument('Experiment');
            
            % Go through all the trials to get a list of unique stimuli
            obj.GetUniqueStimulusList();
            obj.XmlWriteStimulusList(docNode);
            
            % Write all trial blocks
            for blockIdx = 1:length(obj.blockList)
                obj.WriteBlock(docNode, obj.blockList(blockIdx));
            end
            
            xmlwrite('test.xml',docNode);
        end
        
        function GetUniqueStimulusList(obj)
            obj.uniqueStimulusList = cell(20,3);
            nStimuli = 0;
            for blockIdx = 1:length(obj.blockList)
                currentBlock = obj.blockList(blockIdx);
                
                for trialIdx = 1:length(currentBlock.trialList)
                    currentTrial = currentBlock.trialList(trialIdx);
                    
                    for stimulusIdx = 1:length(currentTrial.stimulusList)
                        currentStimulus = currentTrial.stimulusList(stimulusIdx);
                        
                        switch currentStimulus.type
                            case 'image'
                                if isempty(find(strcmp(obj.uniqueStimulusList(:,3),currentStimulus.location)==1,1))
                                    nStimuli = nStimuli + 1;
                                    obj.uniqueStimulusList{nStimuli,1} = 'image';
                                    obj.uniqueStimulusList{nStimuli,2} = nStimuli;
                                    obj.uniqueStimulusList{nStimuli,3} = currentStimulus.location;
                                end
                            case 'aperture_grating'
                                if isempty(find(strcmp(obj.uniqueStimulusList(:,1),'aperture_grating')==1,1))
                                    nStimuli = nStimuli + 1;
                                    obj.uniqueStimulusList{nStimuli,1} = 'aperture_grating';
                                    obj.uniqueStimulusList{nStimuli,2} = nStimuli;
                                end
                            case 'gabor_grating'
                                if isempty(find(strcmp(obj.uniqueStimulusList(:,1),'gabor_grating')==1,1))
                                    nStimuli = nStimuli + 1;
                                    obj.uniqueStimulusList{nStimuli,1} = 'gabor_grating';
                                    obj.uniqueStimulusList{nStimuli,2} = nStimuli;
                                end
                        end
                        
                        if size(obj.uniqueStimulusList,1) == nStimuli
                            tempList = obj.uniqueStimulusList;
                            obj.uniqueStimulusList = cell(nStimuli+20,3);
                            obj.uniqueStimulusList{1:nStimuli,:} = tempList;
                        end
                    end
                end
            end
            obj.uniqueStimulusList = obj.uniqueStimulusList(1:nStimuli,:);
            
        end
        function XmlWriteStimulusList(obj,docNode)
            stimulusListElement = docNode.createElement('StimulusList');
            docNode.getDocumentElement.appendChild(stimulusListElement);
            
            for stimulusIdx = 1:size(obj.uniqueStimulusList,1)
                currentStimulus = docNode.createElement('Stimulus');
                
                stimulusID = docNode.createElement('ID');
                stimulusID.appendChild(docNode.createTextNode(num2str(cell2mat(obj.uniqueStimulusList(stimulusIdx,2)))));
                currentStimulus.appendChild(stimulusID);
                
                stimulusType = docNode.createElement('StimulusType');
                stimulusType.appendChild(docNode.createTextNode(obj.uniqueStimulusList(stimulusIdx,1)));
                currentStimulus.appendChild(stimulusType);
                if strcmp(obj.uniqueStimulusList(stimulusIdx,1),'image')
                    stimulusLocation = docNode.createElement('Location');
                    stimulusLocation.appendChild(docNode.createTextNode(obj.uniqueStimulusList(stimulusIdx,3)));
                    currentStimulus.appendChild(stimulusLocation);
                end
                
                stimulusListElement.appendChild(currentStimulus);
            end
        end
        
        function WriteBlock(obj, docNode, blockObject)
            % Write block header information
            blocklistElement = docNode.createElement('Block');
            blocklistElement.setAttribute('ShuffleOrder',blockObject.shuffleOrder);
            blocklistElement.setAttribute('Repeat','1');
            docNode.getDocumentElement.appendChild(blocklistElement);
            
            % Write trial information
            for trialIdx = 1:length(blockObject.trialList)
                obj.XmlWriteTrial(docNode, blockObject.trialList(trialIdx),blocklistElement);
            end
        end
        
        function XmlWriteTrial(obj, docNode, trialObject, blockElement)
            trialElement = docNode.createElement('Trial');
            
            % Write trial header information
            trialOrderElement = docNode.createElement('TrialOrder');
            trialConditionElement=docNode.createElement('TrialCondition');
            
            trialOrderElement.appendChild(docNode.createTextNode(num2str(trialObject.order)));
            trialConditionElement.appendChild(docNode.createTextNode(num2str(trialObject.condition)));
            trialElement.appendChild(trialOrderElement);
            trialElement.appendChild(trialConditionElement);
            
            % Write reward and transition information
            obj.XmlWriteTransition(docNode, trialObject.transitionObject, trialElement);
            obj.XmlWriteReward(docNode, trialObject.rewardObject, trialElement);
            
            % Write stimulus information
            for stimulusIdx = 1:length(trialObject.stimulusList)
                obj.XmlWriteStimulus(docNode, trialObject.stimulusList(stimulusIdx), trialElement);
            end
            
            blockElement.appendChild(trialElement);
        end
        
        % Write transition parameters to xml
        function XmlWriteTransition(obj, docNode, transitionObject, trialElement)
            transitionElement = docNode.createElement('Transition');
            transitionElement.setAttribute('type',transitionObject.type);
            
            switch transitionObject.type
                case 'velocity'
                    thresholdVelocity = docNode.createElement('ThresholdVelocity');
                    thresholdDuration = docNode.createElement('ThresholdDuration');
                    thresholdComparator = docNode.createElement('ThresholdComparator');
                    
                    thresholdVelocity.appendChild(docNode.createTextNode(num2str(transitionObject.thresholdVelocity)));
                    thresholdDuration.appendChild(docNode.createTextNode(num2str(transitionObject.thresholdDuration)));
                    tresholdComparator.appendChild(docNode.createTextNode(transitionObject.thresholdComparator));
                    
                    transitionElement.appendChild(thresholdVelocity);
                    transitionElement.appendChild(thresholdDuration);
                    transitionElement.appendChild(thresholdComparator);
                case 'duration'
                    trialDuration = docNode.createElement('Duration');
                    
                    trialDuration.appendChild(docNode.createTextNode(num2str(transitionObject.trialDuration)));
                    
                    transitionElement.appendChild(trialDuration);
            end
            
            trialElement.appendChild(transitionElement);
        end
        
        % Write reward parameters to xml
        function XmlWriteReward(obj,docNode, rewardObject, trialElement)
            rewardElement = docNode.createElement('Reward');
            rewardElement.setAttribute('type',rewardObject.type);
            
            switch rewardObject.type
                case 'velocity'
                    thresholdVelocity = docNode.createElement('ThresholdVelocity');
                    thresholdDuration = docNode.createElement('ThresholdDuration');
                    thresholdComparator = docNode.createElement('ThresholdComparator');
                    
                    thresholdVelocity.appendChild(docNode.createTextNode(num2str(rewardObject.thresholdVelocity)));
                    thresholdDuration.appendChild(docNode.createTextNode(num2str(rewardObject.thresholdDuration)));
                    thresholdComparator.appendChild(docNode.createTextNode(rewardObject.thresholdComparator));
                    
                    rewardElement.appendChild(thresholdVelocity);
                    rewardElement.appendChild(thresholdDuration);
                    rewardElement.appendChild(thresholdComparator);
                case 'interval'
                    intervalDuration = docNode.createElement('IntervalDuration');
                    repeatInterval = docNode.createElement('RepeatInterval');
                    
                    intervalDuration.appendChild(docNode.createTextNode(num2str(rewardObject.intervalDuration)));
                    repeatInterval.appendChild(docNode.createTextNode(rewardObject.repeatInterval));
            end
            
            trialElement.appendChild(rewardElement);
        end
        
        
        % Adds stimulus object information to xml file
        function XmlWriteStimulus(obj, docNode, stimulusObject, trialElement)
            stimulusElement = docNode.createElement('Stimulus');
            
            % Write stimulus specific parameters
            if strcmp(stimulusObject.type, 'image')
                stimulusId = find(strcmp(obj.uniqueStimulusList(:,3),stimulusObject.location)==1,1);
                stimulusId = obj.uniqueStimulusList{stimulusId,2};
                
            elseif strcmp(stimulusObject.type, 'aperture_grating') || strcmp(stimulusObject.type,'gabor_grating')
                stimulusId = find((strcmp(obj.uniqueStimulusList(:,1),'aperture_grating') |...
                                  strcmp(obj.uniqueStimulusList(:,1),'gabor-grating'))==1 ,1);
                stimulusId = obj.uniqueStimulusList{stimulusId,2};
                orientation = docNode.createElement('orientation');
                spatialFrequency = docNode.createElement('spatialFrequency');
                phase = docNode.createElement('phase');
                radius= docNode.createElement('radius');
                
                orientation.appendChild(docNode.createTextNode(num2str(stimulusObject.orientation)));
                spatialFrequency.appendChild(docNode.createTextNode(num2str(stimulusObject.spatialFrequency)));
                phase.appendChild(docNode.createTextNode(num2str(stimulusObject.spatialFrequency)));
                radius.appendChild(docNode.createTextNode(num2str(stimulusObject.radius)));
                
                stimulusElement.appendChild(orientation);
                stimulusElement.appendChild(spatialFrequency);
                stimulusElement.appendChild(phase);
                stimulusElement.appendChild(radius);
            end
            stimulusElement.setAttribute('id',num2str(stimulusId));
            
            % Write general stimulus parameters
            startDisplay = docNode.createElement('StartDisplay');
            stopDisplay = docNode.createElement('StopDisplay');
            top = docNode.createElement('Top');
            left =docNode.createElement('Left');
            width = docNode.createElement('Width');
            height = docNode.createElement('Height');
            
            startDisplay.appendChild(docNode.createTextNode(num2str(stimulusObject.startDisplay)));
            stopDisplay.appendChild(docNode.createTextNode(num2str(stimulusObject.stopDisplay)));
            top.appendChild(docNode.createTextNode(num2str(stimulusObject.top)));
            left.appendChild(docNode.createTextNode(num2str(stimulusObject.left)));
            width.appendChild(docNode.createTextNode(num2str(stimulusObject.width)));
            height.appendChild(docNode.createTextNode(num2str(stimulusObject.height)));
            
            stimulusElement.appendChild(startDisplay);
            stimulusElement.appendChild(stopDisplay);
            stimulusElement.appendChild(top);
            stimulusElement.appendChild(left);
            stimulusElement.appendChild(width);
            stimulusElement.appendChild(height);
            
            trialElement.appendChild(stimulusElement);
            
        end
    end
    
end