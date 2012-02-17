
f = 440;
M = 255;
fs = 8192;

rect = ones(1, M);

sinusoid = .6 * cos( ((f * 2 * pi) / fs) * (0:(M-1)) );

sin_padded = zeropadzerophasewin(sinusoid, rect, 5 * M);

sinTr = fft( sin_padded );

stem( [0: fs/(5*M) : fs*(1 - 1 / (5*M))] - (fs/2),...
    abs(fftshift(sinTr))/max(abs(fftshift(sinTr))));