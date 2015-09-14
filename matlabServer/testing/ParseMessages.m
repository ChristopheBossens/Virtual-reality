function [ output_args ] = ParseMessages( obj, event )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
% Byte codes:
% 1 Get experiment folder contents
% 2 Set new experiment folder
% 3 Send images
%    
    while (obj.BytesAvailable > 0)
        % Read signal byte
        signalByte = fread(obj,1);
        % Discard terminator byte
        fread(obj,1);
        switch signalByte
            case 1
                % Get configuration files
                configurations = GetAvailableConfigurations();
                if ~isempty(configurations)
                    SendConfigurations(configurations,obj);
                end

            case 2
                % Add new configuration file
                PrepareConfigFile(obj);
                
            case 3
                % Add image to existing configuration
                AddImage(obj);
        end    
    end
end

