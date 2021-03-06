function Hd = highpass(N, Fpass, Fstop, Wpass, Wstop)
%HIGHPASS Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 8.1 and the Signal Processing Toolbox 6.19.
% Generated on: 24-Nov-2013 18:25:17

% FIR least-squares Highpass filter designed using the FIRLS function.

% All frequency values are in Hz.
Fs = 49164;  % Sampling Frequency

%N     = 5;     % Order
%Fstop = 4000;  % Stopband Frequency
%Fpass = 5000;  % Passband Frequency
%Wstop = 0.1;   % Stopband Weight
%Wpass = 1;     % Passband Weight

% Calculate the coefficients using the FIRLS function.
b  = firls(N, [0 Fstop Fpass Fs/2]/(Fs/2), [0 0 1 1], [Wstop Wpass], ...
           'Hilbert');
Hd = b;

% [EOF]
