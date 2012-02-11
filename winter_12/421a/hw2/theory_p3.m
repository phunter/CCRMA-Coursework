% Music 421a
% Homework 2
% Problem 3

% Blackman window
N = 1024; % window length
wBlackman = (.42 - .5*cos(2*pi*(0:N-1)/(N-1)) + .08*cos(4*pi*(0:N-1)/(N-1)))';
% wBlackman = boxcar(N);

f = 2000;
sr = 8192;
sinusoid = cos( (f * (2 * pi) / sr) * (0:(N-1)) + .25*2*pi);

wSinusBlackman = sinusoid .* wBlackman';

wTrBlackman = fft(wSinusBlackman,4*N);

wTrHalf = [ wTrBlackman(1 : ((4*N)/2)) zeros(1, (4*N)/2) ];

wHalfTime = ifft(wTrHalf);

figure;

subplot(411);
plot(wBlackman);

subplot(412);
plot([0 : 2 * pi/(N*4) : 2 * pi*(1-1/(N*4))] - pi, 20*log10(abs(wTrBlackman)/max(abs(wTrBlackman))));

subplot(413);
wTfShift = fftshift(wTrHalf);
plot([0 : 2 * pi/(N*4) : 2 * pi*(1-1/(N*4))] - pi, 20*log10(abs(wTfShift)/max(abs(wTfShift))));

subplot(414);
plot(abs(wHalfTime(1 : length(wHalfTime)/4)));