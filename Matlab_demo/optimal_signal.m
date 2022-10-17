clear all
parent_path = 'D:\year_3_fall_2022\Embedded\Projects\ESP_Speech_Recognition\sample_audio\';
% Read signals
[S1, Fs1] = audioread(strcat(parent_path, 'Cube1.m4a'));
[S2, Fs2] = audioread(strcat(parent_path, 'Cube2.m4a'));
[S3, Fs3] = audioread(strcat(parent_path, 'Cube3.m4a'));
[S4, Fs4] = audioread(strcat(parent_path, 'Rubik.m4a'));

% Which signal has the maximum length
max_len = max( [ length(S1) length(S2) length(S3)]);

% Add zero padding to make them the same length
switch max_len
    case length(S1)
        S2 = [S2 ; zeros(max_len - length(S2),1)];
        S3 = [S3 ; zeros(max_len - length(S3),1)];
        S4 = [S4 ; zeros(max_len - length(S4),1)];
    case length(S2)
        S1 = [S1 ; zeros(max_len - length(S1),1)];
        S3 = [S3 ; zeros(max_len - length(S3),1)];
        S4 = [S4 ; zeros(max_len - length(S4),1)];
    case length(S3)
        S2 = [S2 ; zeros(max_len - length(S2),1)];
        S1 = [S1 ; zeros(max_len - length(S1),1)];
        S4 = [S4 ; zeros(max_len - length(S4),1)];
    case length(S4)
        S1 = [S1 ; zeros(max_len - length(S1),1)];
        S2 = [S2 ; zeros(max_len - length(S2),1)];
        S3 = [S3 ; zeros(max_len - length(S3),1)];
end


% Convert signals to freq domain
S1_fft = fft(S1);
S2_fft = fft(S2);
S3_fft = fft(S3);
S4_fft = fft(S4);

% Get magnitude of each component
S1_fft_mag = abs(S1_fft);
S2_fft_mag = abs(S2_fft);
S3_fft_mag = abs(S3_fft);
S4_fft_mag = abs(S4_fft);

% 3xn     *     nx1 = 3x1 
% A        *       X  = B
signals_matrix = [S1_fft_mag'; S2_fft_mag'; S3_fft_mag'];
b = ones(3,1);

the_chosen_one = find_optimal_vector(signals_matrix, b)





