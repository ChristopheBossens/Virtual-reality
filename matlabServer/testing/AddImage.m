function [ output_args ] = AddImage( localServer )
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
    confNameLength = fread(localServer,1,'uint16');
    confName = fread(localServer,confNameLength,'char');
    confName = char(confName');
    
    imageCount = fread(localServer,1,'uint16');
    
    display(['Uploading images for: ' confName]);
    for imageIndex = 1:imageCount
        imageNameLength = fread(localServer,1,'uint16');
        imageName = fread(localServer,imageNameLength,'char');
        imageName = char(imageName');

        fileLength = fread(localServer,1,'uint32');
        
        packetSize = 512;
        nPackets = ceil(fileLength/packetSize);
        bytesLeft = fileLength;
        file = [];
        display(['Image ' num2str(imageIndex) ' out of ' num2str(imageCount) ': ' num2str(fileLength) ' bytes.']);
        for packetIndex = 1:nPackets
            if packetSize > bytesLeft;
                packetSize = bytesLeft;
            end
            file = [file; fread(localServer,packetSize,'char')];
            bytesLeft = bytesLeft-512;
        end

        fid = fopen(['C:\VR_SYSTEM\Experiments\' confName '\stimuli\' imageName],'w+');
        fwrite(fid,file);
        fclose(fid);
    end
    
    display('Finished uploading images');
end

