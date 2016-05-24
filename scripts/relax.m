function relax( F, pos, fgws, N, N_plot, source )
%RELAX Plots relaxation of matrix F
%   Detailed explanation goes here
close all

% Transpose the values
F = F';
fgws = fgws';
source =source';

% Get k-eigenvalue
[V, D] = eigs(F);
k = D(1,1);

% Get asymptotic solution
infsum = sum(source);

% Plot final value
figure
h = semilogy( pos, source / max(source),'k-');
hold on
grid on

% Initialize total source S vector
S = [];

x = fgws;
cursum = sum(x);
for n = 0:N
    if( n < N_plot )
        if( n == N_plot -1 )
            h = [h semilogy( pos, x * infsum / cursum / max(source),'k--' )];
        else
            h = [h semilogy( pos, x * infsum / cursum / max(source),'k:' )];
        end
    end
    prevsum = cursum;
    x = F * x / k;
    cursum = sum(x);
    S = [S abs(cursum - prevsum)/prevsum];
end
if ( N_plot > 1 )
    legend([h(1) h(2) h(end)],{'Fission Source','Previous Iterations','Most Recent Iteration'},...
        'location','northwest')
else
    legend([h(1) h(end)],{'Fission Source', 'Most Recent Iteration'},...
        'location','northwest')
end
xlabel('Position (cm)')
ylabel('Normalized Source Rate')
set(gca,'YLim',[0 inf])
set(gca,'XLim',[0 30])

set(gcf, 'PaperPosition', [0 0 5 4]);
set(gcf, 'PaperSize', [5 4]);
saveas(gcf, 'figure', 'pdf')

figure
semilogy(S,'-xk')
xlabel('Iteration')
ylabel('Relative Error')
set(gca,'XLim',[1 30])
grid on

% % Save to PDF
% set(gcf, 'PaperPosition', [0 0 5 4]);
% set(gcf, 'PaperSize', [5 4]);
% saveas(gcf, 'figure', 'pdf')


end