% Music 421a
% Hunter McCurry
%
% Lab 2
% Problem 3

% Part A

sr = 8192;

window_len = 51;
total_len = 256;

w = hann(window_len);
rect = ones(1, window_len);

w_padded = zeropadzerophasewin(w', rect, total_len);

wTr = fft(w_padded);

% stem();

figure(1);
subplot(211);
stem(w_padded);
ax1 = gca;
% set(ax1,'XColor','k','YColor','k')

xlabel('Sample number');

ax2 = axes('Position',get(ax1,'Position'),...
            'XAxisLocation','top',...
            'Color','none',...
            'XColor','k','YColor','k');

axis(ax2,[0 total_len/sr 0 1]);
xlabel('Time in seconds');


% Part B
subplot(212);
plot( [0: sr/total_len: sr*(1-1/total_len)]-(sr/2), 20*log10(abs(fftshift(wTr))/max(abs(fftshift(wTr)))));
title('Frequency response for length 51 Hann window at sampling rate 8192');
xlabel('Frequency in Hz');

% Part C
window_len = 91;
w = hann(window_len);
rect = ones(1, window_len);

w_padded = zeropadzerophasewin(w', rect, total_len);

wTr = fft(w_padded);

figure(2);
subplot(211);
stem(w_padded);
ax1 = gca;
% set(ax1,'XColor','k','YColor','k')

xlabel('Sample number');

ax2 = axes('Position',get(ax1,'Position'),...
            'XAxisLocation','top',...
            'Color','none',...
            'XColor','k','YColor','k');

axis(ax2,[0 total_len/sr 0 1]);
xlabel('Time in seconds');

subplot(212);
plot( [0: sr/total_len: sr*(1-1/total_len)]-(sr/2), 20*log10(abs(fftshift(wTr))/max(abs(fftshift(wTr)))));
title('Frequency response for length 91 Hann window at sampling rate 8192');
xlabel('Frequency in Hz');

% Part D
sr = 8000;
w = hann(window_len);
rect = ones(1, window_len);

w_padded = zeropadzerophasewin(w', rect, total_len);

wTr = fft(w_padded);

figure(3);
subplot(211);
stem(w_padded);
ax1 = gca;
% set(ax1,'XColor','k','YColor','k')

xlabel('Sample number');

ax2 = axes('Position',get(ax1,'Position'),...
            'XAxisLocation','top',...
            'Color','none',...
            'XColor','k','YColor','k');

axis(ax2,[0 total_len/sr 0 1]);
xlabel('Time in seconds');

subplot(212);
plot( [0: sr/total_len: sr*(1-1/total_len)]-(sr/2), 20*log10(abs(fftshift(wTr))/max(abs(fftshift(wTr)))));
title('Frequency response for length 91 Hann window at sampling rate 8000');
xlabel('Frequency in Hz');