function [optimalSignal, tolerance] =  training_model(parent_path, fileRegex)
    % Values used to test the function
    %parent_path = 'D:\year_3_fall_2022\Embedded\Projects\ESP_Speech_Recognition\sample_audio\Rubik_cube';
    %fileRegex = 'Cube*.m4a'
    
    % Read signals
    filePattern = fullfile(parent_path, fileRegex);
    files = dir(filePattern);
    % Number of files in the parent folder
    signalsNum = length(files);

    % Signals_time is a matrix to store the audio data in time domain
    % Fs is the sampling frequency of the signals
    signalsTime = [];
    Fs = zeros(1, signalsNum);

    % store signals in matrix, each row is a separate signal
    for k = 1:signalsNum
    baseFileName = files(k).name;
    fullFileName = fullfile(files(k).folder, baseFileName);
    
    % The current iteration reads currSignal and its sampling freq
    [currSignal, currFs] = audioread(fullFileName);
    
    Fs(k) = currFs;
    signalsTime = addRow(signalsTime, currSignal');
    end

    % Get the signals in freq domain and calculate magnitude in freq domain
    signalsFreqComplex = [];
    signalsFreqMagnitude = [];% Store magnitude of the signal in freq domain
    signalsVectorMag=[];% Store the magnitude of the signal vector: sqrt(sum(x_i^2))
    for d = 1:signalsNum
        currSignalTime = signalsTime(d,:);
        currSignalFreq = fft(currSignalTime);
        signalsFreqComplex = addRow(signalsFreqComplex, currSignalFreq);
        signalsFreqMagnitude = addRow(signalsFreqMagnitude, abs(currSignalFreq));
        signalsVectorMag = addRow(signalsVectorMag, sqrt(sum(signalsFreqMagnitude(d, :).^2)));
    end

    % Get dot products of the signals to determine wich is the best signal
    dotProducts =[]; % Matrix to store dot products between all vectors
    for r1 = 1:signalsNum
        % Main vector where the dot product = dot(currVector1, V_r2)
        currVector1 = signalsFreqMagnitude(r1, :)./signalsVectorMag(r1);
        dotProductRow = [];% current row of the dotProducts Matrix
        for r2 = 1:signalsNum
        if r2 == r1
            % No need to get dot product between a signal and itself
            continue
        else
            currVector2 = signalsFreqMagnitude(r2, :)./signalsVectorMag(r1);
            dotProductRow = [dotProductRow dot(currVector1,currVector2)];
            
        end
        end
        dotProducts = addRow(dotProducts, dotProductRow);
    end

    % Find lowest tolerance acceptable and the optimal signal
    optimalSignalIndex = 1;
    minAcceptDotProduct = dotProducts(1,1);
    for b = 1:signalsNum
        optimalDotProductSum = sum(dotProducts(optimalSignalIndex,:));
        % If condition to find the signal with the highest dot products
        if sum(dotProducts(b,:)) > optimalDotProductSum
            optimalSignalIndex = b;
        end
        % If condition to find minimum acceptable dot product (tolerance)
        if min(dotProducts(b,:)) < minAcceptDotProduct
        minAcceptDotProduct =  min(dotProducts(b,:));
        end
    end
    
    % Return final values
    optimalSignal = signalsFreqMagnitude(optimalSignalIndex, :);
    tolerance = minAcceptDotProduct;

% END OF FUNCTION
end









