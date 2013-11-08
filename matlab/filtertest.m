Fs = 48000;
t = 0:1/Fs:1-(1/Fs);
x = cos(300*2*pi*t)+cos(800*2*pi*t);
%plot(t,x);
y = conv(x, filters(2,:));
plot(t, y(1001:49000));
axis([0 .01 -1 1]);
