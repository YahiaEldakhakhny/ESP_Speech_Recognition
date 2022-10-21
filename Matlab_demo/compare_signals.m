% Script to test the training model
parent_path_train = 'D:\year_3_fall_2022\Embedded\Projects\ESP_Speech_Recognition\sample_audio\Rubik_cube';
fileRegex_train = 'Cube*.m4a';

% Get parameters from training model
[optimalSignalFreqMag, tolerance] = training_model(parent_path_train, fileRegex_train);

% Apply parameters to different signals to see if the model recognises
% signals
parent_path_trial = 'D:\year_3_fall_2022\Embedded\Projects\Testing_audio\';
%fileRegex_trial = '*';
fileName ='Cube3.m4a';

% The current iteration reads currSignal and its sampling freq
[currSignal, currFs] = audioread(strcat(parent_path_trial, fileName));
    
% Make sure that the signal is a column vector
[currRows, currColumns] = size(currSignal);
if (currRows == 2) || (currColumns == 2)
    disp("Invalid Signal Size");
    exit;
elseif currRows == 1
    currSignal = currSignal';
end

currSignalFreqMag = abs(fft(currSignal));
    
% Compute dot product between currSignal and nominal signal (optimalSignalFreqMag)
dotProduct = myDotProduct(currSignalFreqMag', optimalSignalFreqMag);
disp(fileName);
if dotProduct <= tolerance
    disp("yup");
       
else
    disp("Nah");
end




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%{
% Read signals
filePattern = fullfile(parent_path_trial, fileRegex_trial);
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
    disp(k);
    disp(fullFileName);
    
    % The current iteration reads currSignal and its sampling freq
    [currSignal, currFs] = audioread(fullFileName);
    
    % Make sure that the signal is a column vector
    [currRows, currColumns] = size(currSignal);
    if (currRows == 2) || (currColumns == 2)
       disp("Invalid Signal Size");
       exit;
    elseif currRows == 1
        currSignal = currSignal';
    end
    
    % Compute dot product between currSignal and nominal signal (optimalSignalFreqMag)
    dotProduct = myDotProduct(currSignal, optimalSignalFreqMag);
    disp(baseFileName);
    if dotProduct <= tolerance
       disp("yup");
       
    else
        disp("Nah");
    end
    
end
%}
