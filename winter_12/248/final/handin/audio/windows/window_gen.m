

fs = 44100;
N = 2048;

Y = hanning(N/2);
Y_shift = fftshift(Y);

Y_insert = [ ones(N/4,1); Y_shift ; ones(N/4,1)];
Y_reshift = fftshift(Y_insert);

% plot(Y_reshift);
wavwrite(Y_reshift,fs,'hanning_extended.wav');