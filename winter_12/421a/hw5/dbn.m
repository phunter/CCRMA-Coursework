function Xdb = dbn(X);
% convert DFT X into normalized dB
Xdb = 20*log10(abs(X)/max(abs(X)));