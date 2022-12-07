function freq_mag = time_to_frq(signal_time)
    f = fft(signal_time);
    freq_mag = abs(f);
end