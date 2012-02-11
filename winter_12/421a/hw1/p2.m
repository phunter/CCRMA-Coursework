% Music 421a
% Lab 1
% Problem 2

% Blackman window
N = 1024; % window length
wBlackman = (.42 - .5*cos(2*pi*(0:N-1)/(N-1)) + .08*cos(4*pi*(0:N-1)/(N-1)))';

freq = 1000;
sr = 8192;
sinusoid = .6 * cos( ((freq * (2 * pi) / sr) * 0:(N-1)) + .25*2*pi);

wSinusBlackman = sinusoid .* wBlackman';

wTrBlackman = fft(wSinusBlackman, 4 * N);

figure;

subplot(421);
plot(wBlackman);

subplot(423);
plot(sinusoid);

subplot(425);
plot(wSinusBlackman);

subplot(427);
plot([0 : 2 * pi/(N*4) : 2 * pi*(1-1/(N*4))] - pi, 20*log10(abs(wTrBlackman)/max(abs(wTrBlackman))));
grid;

%Boxcar
wBoxcar = boxcar(N);

wSinusBoxcar = sinusoid .* wBoxcar';

wTrBoxcar = fft(wSinusBoxcar, 4 * N);

subplot(422);
plot(wBoxcar);

subplot(424);
plot(sinusoid);

subplot(426);
plot(wSinusBoxcar);

subplot(428);
plot([0 : 2 * pi/(N*4) : 2 * pi*(1-1/(N*4))] - pi, 20*log10(abs(wTrBoxcar)/max(abs(wTrBoxcar))));
grid;
