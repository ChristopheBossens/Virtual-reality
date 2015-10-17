function [ output_args ] = ParseMessages( obj, event, experimentManager )
%PARSEMESSAGE TCP/IP message hub
%   This function is called when Ethernet data is available and calls
%   delegates the processing of these data to the correct function

% Byte codes:
% 1 Get experiment folder contents
% 2 Set new experiment folder
% 3 Send images
% 4 Start experiment

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
                
            case 5
                LoadExperimentConfig(obj, experimentManager);
                
            case 6
                RunExperiment(obj, experimentManager);
                
            case 8
                experimentManager.Close();
                clear experimentManager;
                fclose(obj);
                
            case 9 % Start a loop for mapping receptive fields
                MapRF(obj,experimentManager);
                
        end    
    end
end

