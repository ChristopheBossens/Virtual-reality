function PrepareConfigFile( localServer )
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
    nameLength = fread(localServer,1,'uint16');
    name = fread(localServer,nameLength,'char');
    name = char(name')
    
    fileLength = fread(localServer,1,'uint16');
    file = fread(localServer,fileLength,'char');

    mkdir('C:\VR_SYSTEM\Experiments\',name);
    mkdir(['C:\VR_SYSTEM\Experiments\' name '\'],'stimuli');
    mkdir(['C:\VR_SYSTEM\Experiments\' name '\'],'modifiers');
    mkdir(['C:\VR_SYSTEM\Experiments\' name '\'],'runs');
    
    fid = fopen(['C:\VR_SYSTEM\Experiments\' name '\' name '.econ'], 'w+');
    fwrite(fid,file);
    fclose(fid);
end

