

fs = 44100;
N = 2048;

Y = hanning(N);

wavwrite(Y,fs,'hanning.wav');