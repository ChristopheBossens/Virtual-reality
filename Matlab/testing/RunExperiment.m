function [ output_args ] = RunExperiment( localServer, experimentManager )
%RUNEXPERIMENT Runs an experiment
%   Receives the file to which data has to be logged and then runs the
%   experiment. After the experiment a control byte is sent to signal the
%   end of the experiment
    filenameLength = fread(localServer,1,'uint16');
    filename = fread(localServer, filenameLength,'char');
    filename = char(filename');
    
    experimentManager.RunExperiment(filename, localServer);
    
    fwrite(localServer,[7 1],'uint8');
end

