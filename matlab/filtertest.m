Fs = 48000;
t = 0:1/Fs:1-(1/Fs);
x = cos(300*2*pi*t)+cos(1800*2*pi*t);
%plot(t,x);
y = conv(x, filters(1,:));
plot(t, y(100:48099));
axis([0 .01 -1 1]);
