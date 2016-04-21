function [ ] = relax( F, x0, N, ref )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
F = F';
startval = 0.102;
endval = 0.941;
colorspecs = [linspace(startval,endval,N+1)' linspace(startval,startval,N+1)' linspace(endval,startval,N+1)'];

clf
hold on
grid on

[V D] = eig(F);
k = D(1,1);
S = []

x = x0;
for n = 0:N
    plot( x, 'Color', colorspecs(n+1,:) );
    prevsum = sum(x);
    x = F * x / k;
    S = [S abs(prevsum - sum(x))];
end

plot( ref * prevsum / sum(ref), 'k:' );
title('Fixed source in cell adjoint weighted k eigenvalue flux')
ylim([0 0.1*max(ref)]);

end