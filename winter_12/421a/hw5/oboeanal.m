%diary './dia/oboeanal.dia';
%diary on;

% ---------------------- Part (a) ------------------ % 
% Pitch estimate using a pre-specified range of frequency
% for peak finding.
zpf = 5; % desired min zero-padding factor in all FFTs
fn = 'oboe.ff.C4B4.wav';
[x,fs,nbits] = wavread(fn);
nx = length(x);
dur = nx/fs;
disp(sprintf(['Read %s, fs = %f, nbits = %d, ',...
              'length = %d samples = %0.1f sec'],...
             fn,fs,nbits,nx,dur));
%soundsc(x,fs);
M = fs/10; % 100 ms window for pitch estimation
N = 2^nextpow2(M*zpf); % zero pad
win = boxcar(M);
Xzp = fft(x(1:M).*win,N);

figure(1);
fmax = 500; % maximum freq in Hz
kmax = fmax*N/fs; % maximum freq in bins
prange = 1:kmax+1;
Xdb = dbn(Xzp(prange));
f = fs*[0:N-1]/N/1000; % frequency axis in kHz
plot(f(prange),Xdb);
ylabel('Magnitude (dB)');
xlabel('Frequency (kHz)');
[amps,freqs] = findpeaks(Xdb,1,fs,win,N); %---WRITE THIS FUNCTION
%k0 = bins(1)-1;
%f0 = k0*fs/N;
f0=freqs(1);
nP = fs/f0; % samples per period (used below)
disp(sprintf('Estimated pitch = %f Hz',f0));

% Test pitch estimate:
%pause(ceil(dur)); % let previous sound finish
%T = 1/fs;
%t = 0:T:dur;
%y = sawtooth(2*pi*f0*t);
%sound(y,fs)


winnames = {'none','boxcar','none','hamming','none','blackman'};

% -------------------------- Part (b)------------------------- %
% Configure window analysis
% This part shows the spectra of a truncated oboe signal using different
% windows. For K set as default here, the peaks are "just resolvable". 

for K=[2 4 6]
  wintype = winnames{K};
  %%---- try substituting K by (K-1) and (K+1) and comment on resolvability -----%%
  K = K+1;
  M = ceil(K*nP); % min Blackman length
  cmd = sprintf('w = %s(M);',wintype);
  eval(cmd);
  Nw = 2^nextpow2(M*zpf) % zero pad
  xw = x(1:M) .* w;
  Xwzp = fft(xw,Nw);
  %fmax = 5*f0; % maximum freq in Hz
  fmax = fs/4;
  kmax = fmax*Nw/fs; % maximum freq in bins
  prange = 1:kmax+1;
  Xwdb = dbn(Xwzp(prange));

  figure(K);
  subplot(2,1,1);
  plot(xw,'-k');
  axis tight;
  title(sprintf('Oboe data - %s window, K=%d',wintype,K));
  ylabel('Amplitude');
  xlabel('Time (samples)');
  subplot(2,1,2);
  f = fs*[0:Nw-1]/Nw/1000; % frequency axis in kHz
  plot(f(prange),Xwdb,'-k'); grid on;
  axis([f(prange(1)) f(prange(end)) -90 0]);
  ylabel('Magnitude (dB)');
  xlabel('Frequency (kHz)');

  plotfile = sprintf('../eps/oboe%s.eps',wintype);
  %saveplot(plotfile);

end

%diary off;