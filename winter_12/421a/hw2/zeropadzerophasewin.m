function [output_sig] = zeropadzerophasewin(input_sig, window, padded_length)
% function windows and zero-pads a signal
% check that input_sig is the same size vector as the window:
% multiply window by input_sig:
% add zeros to end so that length(output_sig) is padded_length:


if (length(input_sig) ~= length(window))
    error('Input signal and window must be the same length.');
    % hi = 23
end


if (length(input_sig) > padded_length)
    error('Size of input signal must not exceed desired length.');
    
else
    windowed = input_sig .* window;
 
    fillspace = padded_length - length(windowed);
    oddness = mod(fillspace,2);
    
    output_sig =   [zeros(1, floor(fillspace/2) + oddness) windowed zeros(1, floor(fillspace/2))];
    output_sig = fftshift(output_sig);
end
