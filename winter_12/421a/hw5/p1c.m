% Music 421a
% Lab 5
% Problem 1c

[x fs] = wavread('s1.wav');
N = length(x);

Xlin = fft(x);

eps = .00000001;
Xwdb = 20*log10(abs(Xlin)+eps);

subplot(211);
plot([0: fs/N : fs - 1], Xwdb);
title('Magnitude and Phase response of s1.wav with rectangular window');
ylabel('Magnitude (dB)');
xlabel('Frequency (Hz)');
subplot(212);
plot([0: fs/N : fs - 1], angle(Xlin));
ylabel('Normalized Phase');
xlabel('Frequency (Hz)');

[peaks, freqs] = findpeaks(Xwdb,4,fs,boxcar(length(x)),length(x));

peaks
freqs