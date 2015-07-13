classdef StimulusManager < handle
    % Repository for keeping track of different stimulus textures accross
    % different monitors with PTB. Also contains a GenerateFramesFromCounts
    % function that allows to create frame parameters for stimulus
    % presentation.
    
    properties 
        lMonitorTextureIndices = [];
        rMonitorTextureIndices = [];
        stimulusIndexList = [];
        stimulusNameList = {};
        stimulusCount = 0;
        offlineMode = 0;

        lMonitorIndex = -1;
        rMonitorIndex = -1;
    end
    
    methods
        % Class constructor and destructor
        function obj = StimulusManager(lIndex, rIndex)
            % StimulusManager StimulusManager
            %   Constructor for StimulusManager object. Requires that two
            %   monitors are already opened by PTB and that their indices
            %   are passed as arguments to the constructor.
            %
            %   if no arguments are passed, the Manager starts in offline
            %   mode, which can be used to validate the frame parameters
            %   matrix
            
            if nargin ~= 2
                display('No monitor indices supplied. StimulusManager is running offline!');
                obj.offlineMode = 1;
                return;
            end
            obj.lMonitorIndex = lIndex;
            obj.rMonitorIndex = rIndex;
        end
        function delete(obj)
            if (obj.offlineMode == 0)
                for lScreenIndex = 1:length(obj.lMonitorTextureIndices)
                    Screen('Close',obj.lMonitorTextureIndices(lScreenIndex));
                end

                for rScreenIndex = 1:length(obj.rMonitorTextureIndices)
                    Screen('Close',obj.rMonitorTextureIndices(rScreenIndex));
                end 
            end
        end
        
        % Add and remove stimuli from database
        function index = AddStimulus(obj, stimulusName, stimulusFile)
            % StimulusManager AddStimulus
            %  Uploads stimulus with name stimulusName to video memory. A
            %  separate texture needs to be uploaded for each open window,
            %  so each stimulus has different texture id's. Therefore, the
            %  index returned by this function can be used as a single
            %  reference to these textures
            if ( sum(strcmp(stimulusName, obj.stimulusNameList)) > 0)
                display(['Error: a stimulus with the same name is already in the list']);
                return
            end
            stimulus = imread(stimulusFile);          
            
            if obj.offlineMode == 0
                lIndex = Screen('MakeTexture',obj.lMonitorIndex, stimulus);
                rIndex = Screen('MakeTexture',obj.rMonitorIndex, stimulus);
            else
                lIndex = stimulusCount + 1;
                rIndex = stimulusCount + 1;
            end
            
            obj.lMonitorTextureIndices = [obj.lMonitorTextureIndices, lIndex];
            obj.rMonitorTextureIndices = [obj.rMonitorTextureIndices, rIndex];
            
            obj.stimulusCount = obj.stimulusCount + 1;
            obj.stimulusIndexList = [obj.stimulusIndexList obj.stimulusCount];
            obj.stimulusNameList = [obj.stimulusNameList {stimulusName}];
            
            index = obj.stimulusCount;
        end
        function RemoveStimulus(obj, stimulusName)
            if ischar(stimulusName)
                listIndex = find(strcmp(stimulusName, obj.stimulusNameList));
            else
                listIndex = find(obj.stimulusIndexList == stimulusName);
            end
            
            if ~isempty(listIndex)
                if (obj.offlineMode == 0)
                    Screen('Close', obj.lMonitorTextureIndices(listIndex));
                    Screen('Close', obj.rMonitorTextureIndices(listIndex));
                end
                obj.lMonitorTextureIndices(listIndex) = [];
                obj.rMonitorTextureIndices(listIndex) = [];
                obj.stimulusIndexList(listIndex) = [];
                obj.stimulusNameList(listIndex) = [];
            end
        end
        
        % Get indices from name
        function [stimulusIndex] = GetStimulusIndex(obj, stimulusName)
            % StimulusManager GetStimulusIndex
            %   Returns the index for a given stimulus name, or -1 if the
            %   stimulus name is not recognized
            listIndex = find(strcmp(stimulusName, obj.stimulusNameList));
            if ~isempty(listIndex)
                stimulusIndex = obj.stimulusIndexList(listIndex);
            else
                stimulusIndex = -1;
            end
        end
        function [lMonitorIndex] = GetLeftMonitorIndex(obj, stimulusName)
            % StimulusManager GetLeftMonitorIndex
            %   Returns the texture index for a stimulus on the left
            %   monitor. The stimulus can be given as a name or as an index
            if ischar(stimulusName)
                listIndex = find(strcmp(stimulusName, obj.stimulusNameList));
            else
                listIndex = find(obj.stimulusIndexList == stimulusName);
            end
            
            lMonitorIndex = -1;
            if ~isempty(listIndex)
                lMonitorIndex = obj.lMonitorTextureIndices(listIndex);
            end
        end       
        function [rMonitorIndex] = GetRightMonitorIndex(obj, stimulusName)
            % StimulusManager GetRightMonitorIndex
            %   Returns the texture index for a stimulus on the left
            %   monitor. The stimulus can be given as a name or as an index
            if ischar(stimulusName)
                listIndex = find(strcmp(stimulusName, obj.stimulusNameList));
            else
                listIndex = find(obj.stimulusIndexList == stimulusName);
            end
            rMonitorIndex = -1;
            if ~isempty(listIndex)
                rMonitorIndex = obj.rMonitorTextureIndices(listIndex);
            end
        end       

        % Reports which stimulus indices are not in the list
        function [frameParameters] = GenerateFramesFromCounts(obj, stimulusData)
            % StimulusManager GenerateFramesFromCounts
            %  Generates a frames parameters matrix from stimulus data
            %  stimulus data is expected to have the following format
            %  col 1: monitor index
            %  col 2: stimulus order
            %  col 3: stimulus index as returned by AddStimulus method
            %  col 4: number of frames stimulus has to be presented
            % 
            %  Returns a matrix with the following data:
            %  col 1: stimulus ID for monitor 1
            %  col 2: stimulus ID for monitor 2
            %  col 3...: optional columns
            lScreenStimuli = find(stimulusData(:,1) == 1);
            rScreenStimuli = find(stimulusData(:,1) == 2);
            lScreenPresentationTime = sum(stimulusData(lScreenStimuli,4));
            rScreenPresentationTime = sum(stimulusData(rScreenStimuli,4));
            nFrames = max([lScreenPresentationTime rScreenPresentationTime]);

            frameParameters = -1.*zeros(nFrames,10);
            lScreenData = sortrows(stimulusData(lScreenStimuli,:),2);
            rScreenData = sortrows(stimulusData(rScreenStimuli,:),2);

            previousIndex = 1;
            for orderIndex = 1:size(lScreenData,1)
                frameParameters(previousIndex:previousIndex+lScreenData(orderIndex,4)-1,1) = obj.GetLeftMonitorIndex(lScreenData(orderIndex,3));
                previousIndex = previousIndex + lScreenData(orderIndex,4);
            end

            previousIndex = 1;
            for orderIndex = 1:size(rScreenData,1)
                frameParameters(previousIndex:previousIndex+rScreenData(orderIndex,4)-1,2) = obj.GetRightMonitorIndex(rScreenData(orderIndex,3));
                previousIndex = previousIndex + rScreenData(orderIndex,4);
            end

        end
        function naList = ConsistencyCheck(obj, list)
            % StimulusManager ConsistencyCheck
            %   Checks if all indices in a list are available in the
            %   StimulusManager
            naList = NaN(1,length(list));
            for listIndex = 1:length(list)
                if( isempty(find(obj.stimulusIndexList == list(listIndex),1)))
                    naList(listIndex) = list(listIndex);
                end
            end
            naList(isnan(naList)) = [];
        end
        
        % Print summary of active data in stimulus manager
        function Summary(obj)
            % DisplayManager Summary
            %   Gives a summary of all uploaded stimuli in the
            %   StimulusManager
            display('Active stimuli in stimulus manager:');
            for i = 1:obj.stimulusCount
                display(['stimulus ' num2str(i) ': ' obj.stimulusNameList{i} ' ' num2str(obj.lMonitorTextureIndices(i)) ' ' num2str(obj.rMonitorTextureIndices(i))]);
            end
        end
    end
    
end

