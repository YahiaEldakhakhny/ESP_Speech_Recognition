clear all
% Variable declaration
parent_path = 'D:\year_3_fall_2022\Embedded\Projects\ESP_Speech_Recognition\sample_audio\';
S = [];

% Read signals
filePattern = fullfile(parent_path, '*.m4a');
theFiles = dir(filePattern);
for k = 1:length(theFiles)
    baseFileName = theFiles(k).name;
    fullFileName = fullfile(theFiles(k).folder, baseFileName);
    


end

%[curr_signal, sampling_freq] = audioread(fullFileName);
%S = [S curr_signal];

% 3xn     *     nx1 = 3x1 
% A        *       X  = B
%signals_matrix = [S1_fft_mag'; S2_fft_mag'; S3_fft_mag'];
b = ones(3,1);

%the_chosen_one = find_optimal_vector(signals_matrix, b)





