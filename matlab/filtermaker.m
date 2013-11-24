order = 5;
Fstep = 100;
transition_width = 1000;

filters_lp = zeros(5000/Fstep + 1, order);
filters_hp = zeros(5000/Fstep + 1, order);

for f = 1:(5000/Fstep)
    filters_lp(f,:) = lowpass(order-1, f*Fstep, f*Fstep+transition_width, 1, .1);
end

for f = 1:(5000/Fstep)
    if (f*Fstep - transition_width <= 0)
        filters_hp(f,:) = highpass(order-1, f*Fstep, 1, 1, .1);
    else
        filters_hp(f,:) = highpass(order-1, f*Fstep, f*Fstep-transition_width, 1, .1);
    end
end