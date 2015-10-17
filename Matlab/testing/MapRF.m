function MapRF( localServer, experimentManager )
%MapRF Controls receptive field mapping
%   Function that takes x and y coordinates as input
%   and uses those to draw a grating on the screen

    % 0. Prepare screen for gabor drawing
    [window, rect] = experimentManager.GetScreenInformation();
    white = WhiteIndex(window);
    black = BlackIndex(window);
    gray = 0.5*(white+black);
    
    gaborDimPix = 100;
    sigma = gaborDimPix/6;
    orientation = 90;
    contrast = 0.8;
    aspectRatio = 1.0;
    nCycles = 3;
    spatialFreq = nCycles/gaborDimPix;
    gaborTex = CreateProceduralGabor(window, gaborDimPix, gaborDimPix,...
        [], [0.5 0.5 0.5 0.0], 1, 0.5);
    degPerSec = 360;

    startPhase = 0;
    xCenter = 50;
    yCenter = 50;
    
    destRect =   [xCenter/2,yCenter/2, gaborDimPix, gaborDimPix];   
    gaborProperties = [startPhase, spatialFreq, sigma, contrast, aspectRatio, 0, 0, 0]; 
    gaborData = [xCenter-gaborDimPix/2, yCenter-gaborDimPix/2, xCenter+gaborDimPix/2, yCenter+gaborDimPix/2];
    
    display('Ready for RF mapping');
    % Loop for updating and drawing grating parameters
    while 1
        tic;
        fwrite(localServer,[1],'uint8');
        destRect = [gaborData(1)-gaborDimPix/2, gaborData(2)-gaborDimPix/2, gaborData(1)+gaborDimPix/2, gaborData(2)+gaborDimPix/2];
        

        Screen('DrawTexture', window, gaborTex, [], destRect, orientation, [],[],[],[],...
            kPsychDontDoRotation, gaborProperties);
        Screen('Flip',window);
        
        gaborData = fread(localServer,4,'int16');
        if gaborData(1) < 0
            break;
        end
        
        ifi = toc;
        gaborProperties(1) = gaborProperties(1) + degPerSec * ifi;
    end    
    display(['IFI: ' num2str(ifi)])
    % 4. Check end bytes
    Screen('Close',gaborTex);
    fwrite(localServer,[1],'uint8');
    display('Stopped RF mapping');
end

