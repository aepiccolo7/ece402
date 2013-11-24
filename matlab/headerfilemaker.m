order = 5;
Fstep = 100;

fd=fopen('filter_coef.h','wt');
fprintf(fd, '#ifndef FILTER_COEF_H\n');
fprintf(fd, '#define FILTER_COEF_H\n\n');
fprintf(fd,'float h_lp[%d][%d]={ ', 5000/Fstep, order);
for f = 1:(5000/Fstep)
    fprintf(fd,'{%.9g,', filters_lp(f,1));
    fprintf(fd,'%.9g,', filters_lp(f,2:end));
    fprintf(fd,'},\n');
end
fprintf(fd,'};\n\n');

fprintf(fd,'float h_hp[%d][%d]={ ', 5000/Fstep, order);
for f = 1:(5000/Fstep)
    fprintf(fd,'{%.9g,', filters_hp(f,1));
    fprintf(fd,'%.9g,', filters_hp(f,2:end));
    fprintf(fd,'},\n');
end
fprintf(fd,'};\n\n');
fprintf(fd, '#endif');
fclose(fd);