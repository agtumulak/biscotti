function [S] = relax( F, fgws, N, source )
%RELAX Plots relaxation of matrix F
%   Detailed explanation goes here

% Get k-eigenvalue
[V, D] = eig(F);
k = D(1,1);

% Get asymptotic solution
x_inf = (F/k)^1000 * fgws;


% Plot final value
plot( source * sum(x_inf) / sum(source) );
hold on
grid on

% Initialize total source S vector
S = [];

x = fgws;
for n = 0:N
    plot( x );
    prevsum = sum(x);
    x = F * x / k;
    S = [S abs(prevsum - sum(x))/prevsum];
end

figure
semilogy(S,'x')
grid on

end