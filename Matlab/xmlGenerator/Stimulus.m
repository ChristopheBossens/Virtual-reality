classdef Stimulus < handle
    properties
        type;
        location;
        
        % When to start stimulus display
        startDisplay = 0;
        stopDisplay = 0;
        
        % Stimulus position parameters
        top = 0;
        left =0;
        width = 0;
        height =0;
        
        % Parameters for grating stimulus type
        orientation = 0;
        spatialFrequency = 0;
        phase = 0;
        radius = 0;
        
    end
    methods
        function obj = Stimulus(type)
            if nargin ~= 1
                error('Stimulus type not specified')
            end
            
            switch type
                case 'image'
                    obj.type = type;
                case 'aperture_grating'
                    obj.type = type;
                case 'gabor_grating'
                    obj.type = type;
                otherwise
                    error('Stimulus type not recognized')
            end
        end
        
        % Sets the image source location for an image stimulus
        function SetStimulusLocation(obj, stimulusLocation)
            if nargin ~= 2
                error('Insufficient parameters provided')
            end
            
            if strcmp(obj.type,'image') ~= 1
                error('Stimulus is not of type ''image''');
            else
                obj.location = stimulusLocation;
            end
        end
        
        % Sets position coordinates in pixels for the stimulus in the
        % following order:
        %  top, left, width, height
        function SetStimulusPosition(obj, top,left,width,height)
            if nargin ~= 5
                error('Insufficient parameters provided')
            end
            
            obj.top = top;
            obj.left = left;
            obj.width = width;
            obj.height = height;
        end
        
        % Sets the frames at which the stimulus should be presented
        function SetPresentationTime(obj, startDisplay,stopDisplay)
            if nargin ~= 3
                error('Insufficient parameters provided')
            end
            
            obj.startDisplay = startDisplay;
            obj.stopDisplay = stopDisplay;
        end
        
        % For grating stimuli, specify additional parameters
        function SetGratingParameters(obj, orientation, sf, phase,radius)
            if nargin ~= 5
                error('Insufficient parameters provided')
            end
            
            if strcmp(obj.type, 'aperture_grating') == 1 ||strcmp(obj.type,'gabor_grating') == 1
                obj.orientation = orientation;
                obj.spatialFrequency = sf;
                obj.phase = phase;
                obj.radius = radius;
            else
                error('Stimulus is not of type ''aperture_grating'' or ''gabor_grating''');
            end
        end
    end
end