order = 100;
Fstep = 100;
transition_width = 1000;

filters = zeros(5000/Fstep + 1, order);
for f = 1:(5000/Fstep)
    filters(f,:) = lowpass(order-1, f*Fstep, f*Fstep+transition_width, 1, 1);
end
