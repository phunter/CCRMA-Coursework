function [output_sig] = zeropadwin(input_sig, window, padded_length)
% function windows a signal and adds zeros to the end
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
    output_sig = [windowed zeros(1, padded_length-length(windowed))];
end