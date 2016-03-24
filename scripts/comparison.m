function [] = comparison()
%COMPARISON Compares the results of two files
%   Detailed explanation goes here

%% Load Data
forward = load( '/Users/atumulak/Developer/sn-solver/outputs/case0.mat' );
adjoint = load( '/Users/atumulak/Developer/sn-solver/outputs/case1.mat' );

clf
hold on
grid on

%% Plot slow
factor = sum(forward.sn_scalar_flux_group_0_point_1_mev)/sum(adjoint.sn_scalar_flux_group_0_point_1_mev);
plot( forward.sn_scalar_flux_group_0_point_1_mev, '-' );
plot( factor * adjoint.sn_scalar_flux_group_0_point_1_mev, '-' );

ax = gca;
ax.ColorOrderIndex = 1;

%% Plot fast
factor = sum(forward.sn_scalar_flux_group_14_mev)/sum(adjoint.sn_scalar_flux_group_14_mev);
plot( forward.sn_scalar_flux_group_14_mev, '--' );
plot( factor * adjoint.sn_scalar_flux_group_14_mev, '--' );

%% Legend
legend('forward slow','adjoint slow','forward fast','adjoint fast')


% % Save to PDF
% set(gcf, 'PaperPosition', [0 0 10 10]);
% set(gcf, 'PaperSize', [10 10]);
% saveas(gcf, 'scattering-ratio-comparison', 'pdf')


end

