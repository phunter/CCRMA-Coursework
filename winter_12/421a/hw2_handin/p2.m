% Music 421a
% Hunter McCurry
%
% Lab 2
% Problem 2

% Part A

sr = 8192;

window_len = 51;
total_len = 256;

rect = ones(1, window_len);

rect_padded = zeropadzerophasewin(rect, rect, total_len);

rectTr = fft(rect_padded);

figure(1);
subplot(211);
stem(rect_padded);
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
plot( [0: sr/total_len: sr*(1-1/total_len)]-(sr/2), 20*log10(abs(fftshift(rectTr))/max(abs(fftshift(rectTr)))));
title('Frequency response for length 51 rectangular window at sampling rate 8192');
xlabel('Frequency in Hz');

% Part C
window_len = 91;
rect = ones(1, window_len);
 
rect_padded = zeropadzerophasewin(rect, rect, total_len);
 
rectTr = fft(rect_padded);

figure(2);
subplot(211);
stem(rect_padded);
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
plot( [0: sr/total_len: sr*(1-1/total_len)]-(sr/2), 20*log10(abs(fftshift(rectTr))/max(abs(fftshift(rectTr)))));
title('Frequency response for length 91 rectangular window at sampling rate 8192');
xlabel('Frequency in Hz');

% Part D

sr = 8000;
window_len = 91;
rect = ones(1, window_len);

rect_padded = zeropadzerophasewin(rect, rect, total_len);

rectTr = fft(rect_padded);

figure(3);
subplot(211);
stem(rect_padded);
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
plot( [0: sr/total_len: sr*(1-1/total_len)]-(sr/2), 20*log10(abs(fftshift(rectTr))/max(abs(fftshift(rectTr)))));
title('Frequency response for length 91 rectangular window at sampling rate 8000');
xlabel('Frequency in Hz');