order = 100;
Fstep = 100;

fd=fopen('filter_coef.h','wt');
fprintf(fd,'float h[%d][%d]={ ', 5000/Fstep, order);
for f = 1:(5000/Fstep)
    fprintf(fd,'{%.9g,', filters(f,1));
    fprintf(fd,'%.9g,', filters(f,2:end));
    fprintf(fd,'},\n');
end
fprintf(fd,'};\n');
fclose(fd);