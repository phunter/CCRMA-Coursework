function d=dissmeasure(fvec,amp)
%
% given a set of partials in fvec,
% with amplitudes in amp,
% this routine calculates the dissonance
%
Dstar=0.24; S1=0.0207; S2=18.96; C1=5; C2=-5;
A1=-3.51; A2=-5.75; firstpass=1;
N=length(fvec);
[fvec,ind]=sort(fvec);
ams=amp(ind);
D=0;
for i=2:N
Fmin=fvec(1:N-i+1);
S=Dstar./(S1*Fmin+S2);
Fdif=fvec(i:N)-fvec(1:N-i+1);
a=min(ams(i:N),ams(1:N-i+1));
Dnew=a.*(C1*exp(A1*S.*Fdif)+C2*exp(A2*S.*Fdif));
D=D+Dnew*ones(size(Dnew))';
end
d=D;