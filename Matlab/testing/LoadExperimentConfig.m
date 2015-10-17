function LoadExperimentConfig( localServer, experimentManager )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
    filenameLength = fread(localServer,1,'uint16');
    filename = fread(localServer, filenameLength,'char');
    filename = char(filename');
    
    display(['Received filename: ' filename]);
    testXML = experimentManager.LoadXML(filename);
    testTex = experimentManager.PrepareTextures();
    
    fwrite(localServer,[5 (testXML+testTex)],'uint8');
end

