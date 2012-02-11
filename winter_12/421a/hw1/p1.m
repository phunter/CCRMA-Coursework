% Music 421a
% Lab 1
% Problem 1

M=15;
Nfft = 1024;
n=-(M-1)/2:(M-1)/2;
wtrian = 1-abs(n)*2/(M-1);
nzeros = (Nfft-length(wtrian)-1)/2;
wtrian = fftshift([zeros(1,nzeros+1) wtrian zeros(1,nzeros)]);
Wtr=fftshift(fft(wtrian));
figure;
title('Magnitude and phase response of triangular window-M=15');
subplot(211);
plot([0:2*pi/Nfft:2*pi*(1-1/Nfft)]-pi,20*log10(abs(Wtr)/max(abs(Wtr))));
xlabel('Normalized frequency');
ylabel('Magnitude (dB)');
axis([-pi pi ylim]);grid;
subplot(212);
plot([0:M/Nfft:M*(1-1/Nfft)]-M/2,angle(Wtr));
xlabel('Normalized frequency');
ylabel('Phase');axis([-pi pi ylim]);
grid;