
wTr = fft(h);
N = length(h)

plot([0 : 2 * pi/(N) : 2 * pi*(1-1/(N))], 20*log10(abs(wTr)/max(abs(wTr))));
grid;
title('Frequency Response of ir.mat');
xlabel('Normalized Frequency');
ylabel('Amplitude (dB)');
