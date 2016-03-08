function [] = comparison()
%COMPARISON Compares the results of two files
%   Detailed explanation goes here

%% Load Data
%reference = load( '/Users/atumulak/Developer/old/sn-solver-2/outputs/case1.mat' );
test = load( '/Users/atumulak/Developer/sn-solver/outputs/case1.mat' );

clf
hold on
grid on

%% Plot slow
%plot( reference.sn_scalar_flux_group_0_point_1_mev, '-' );
plot( test.sn_scalar_flux_group_0_point_1_mev, '--' );

ax = gca;
ax.ColorOrderIndex = 1;

%% Plot fast
%plot( reference.sn_scalar_flux_group_14_mev, '-' );
plot( test.sn_scalar_flux_group_14_mev, '--' );

%% Legend
%legend('reference slow','test slow','reference fast','test fast')


% % Save to PDF
% set(gcf, 'PaperPosition', [0 0 10 10]);
% set(gcf, 'PaperSize', [10 10]);
% saveas(gcf, 'scattering-ratio-comparison', 'pdf')


end

