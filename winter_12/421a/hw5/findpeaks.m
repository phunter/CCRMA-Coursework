function [peaks,freqs]=findpeaks(Xwdb,maxPeaks,fs,win,N)
% peaks = a vector containing the peak magnitude estimates (linear) using
%         parabolic interpolation in order from largest to smallest peak.
% freqs = a vector containing the frequency estimates (Hz) corresponding
%         to the peaks defined above
% Xwdb  = DFT magnitude (in dB scale) vector of a windowed signal.
%         NOTE that it may contain
%         only negative-frequency (length < N/2+1), positive-frequency
%         (length = N/2+1), or all (length = N) bins of the FFT.
% maxPeaks = the number of peaks we are looking for
% fs = sampling frequency in Hz
% win = window used to obtain Xwdb (assumed zero phase)
% N = NFFT, the number of points used in the FFT creating Xwdb

if (length(Xwdb) == N)
    Xwdb = Xwdb(1:floor(N/2));
end

%-- Find all peaks (magnitudes and indices) by comparing each point of ---%
%-- magnitude spectrum with its two neighbors ---%
allPeaks = [];

for i=2:length(Xwdb)-1
    if (Xwdb(i) > Xwdb(i-1) && Xwdb(i) > Xwdb(i+1))
        newPeak = [Xwdb(i) ; i];
        allPeaks = [allPeaks newPeak];
    end
end


%-- Order from largest to smallest magnitude, keep only maxPeaks of them --%
[Mags, Inds] = sort(allPeaks(1,:), 2, 'descend');
peak_matrix = allPeaks(:,Inds);

%-- Resize to only include the first maxPeaks columns --%
peak_matrix(:, 4:size(peak_matrix,2)) = [];

freq_indx = [];
db_peaks = [];

%-- Do parabolic interpolation in dB magnitude to find more accurate peak --%
%-- and frequency estimates --%
for i=1:min(length(peak_matrix), maxPeaks)
    idx=peak_matrix(2,i);
    %parabolic interpolation
    a=Xwdb(idx-1);
    b=Xwdb(idx);
    c=Xwdb(idx+1);
    [p,y,a] = qint(a,b,c);
    db_peaks = [db_peaks, y];
    freq_indx = [freq_indx, (idx + p)];
end
    
%-- Return linear amplitude and frequency in Hz --%
% NOTE that we must use knowledge of the window to normalize amplitude here
% if we have a TD cosine of amplitude 0.6, this output should be 0.6
win_max = max(win);

lin_peaks = db2mag(db_peaks);
peak_max = max(lin_peaks);

peaks = win_max * (lin_peaks / peak_max);
freqs = (freq_indx-1) * (fs / N);
