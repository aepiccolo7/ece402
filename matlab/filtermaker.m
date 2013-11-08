order = 1001;
Fstep = 100;

filters = zeros(5000/Fstep + 1, order);
for f = 1:(5000/Fstep)
    filters(f,:) = lowpass(order-1, f*Fstep, f*Fstep+100, 1, 1);
end
