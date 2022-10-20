% Define folder that contains audio samples
parent_path = 'D:\year_3_fall_2022\Embedded\Projects\ESP_Speech_Recognition\sample_audio\Rubik_cube';

% Read signals
filePattern = fullfile(parent_path, 'Cube*.m4a');
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
   signalsTime
end




