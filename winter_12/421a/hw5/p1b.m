% Music 421a
% Lab 5
% Problem 1b

fs = 8000;
T = 1/fs;

f = 400;

n = (-127: 127);
sinusoid = cos( 2 * pi * f * n * T);

zpzp_Xw = fft(zpzpwin(cos(2*pi*400/8000*(-127:127)'),boxcar(255),2048));

subplot(211);
plot([0: fs/2048 : fs - 1], 20*log10(abs(zpzp_Xw)));
title('Magnitude and Phase response of 400 Hz cosine signal');
ylabel('Magnitude (dB)');
xlabel('Frequency (Hz)');
subplot(212);
plot([0: fs/2048 : fs - 1], angle(zpzp_Xw));
ylabel('Normalized Phase');
xlabel('Frequency (Hz)');

[peaks, freqs] = findpeaks(20*log10(abs(zpzp_Xw)),2,8000,boxcar(255),2048);

peaks
freqs