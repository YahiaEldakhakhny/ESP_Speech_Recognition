% Read signals
[S1, Fs1] = audioread('Cube1.m4a');
% Calculate signal length
len = length(S1);
% End time of the signal
et = len/Fs1;
t = linspace(0, et, len);
% Get freq_vec
freq_vec = linspace(-Fs1/2, Fs1/2, len);
% Get filter
my_filter = ones(len, 1);
for i = 1:length(my_filter)
    if abs(freq_vec(i)) > 6000
        my_filter(i) = 0;
    end
end
%play signal
%sound(S1, Fs1)
%plot signal in time domain
figure
plot(t, S1)

% Get freq domain of signal
S1_fft = fftshift(fft(S1));

% plot signal in freq domain with no filter
figure
plot(freq_vec, abs(S1_fft));

% Apply filter to signal in freq domain
S1_fft = abs(S1_fft).* my_filter;

%Plot signal after filter in freq domain
%figure
%plot(freq_vec, S1_fft);


% Return filtered signal to time domain
S1_filtered = ifft(ifftshift(S1_fft));

%plot signal in time domain
figure
plot(t, S1_filtered)

