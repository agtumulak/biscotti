// slab.cpp
// Aaron G. Tumulak

// std includes
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

// sn-solver includes
#include "cell.hpp"
#include "groupdependent.hpp"
#include "layout.hpp"
#include "settings.hpp"
#include "slab.hpp"

// Default constructor
Slab::Slab( const Settings &settings, const Layout &layout ):
    settings_( settings ),
    layout_( layout ),
    cur_k_( settings_.KGuess() ),
    adj_cur_k_( settings_.AdjKGuess() ),
    cur_fission_source_( settings_.FissionSourceGuess() ),
    adj_cur_fission_source_( settings_.AdjFissionSourceGuess() ),
    cells_( layout_.GenerateCells( settings_, cur_k_, adj_cur_k_ ) ),
    energy_groups_( layout_.GenerateEnergyGroups() ),
    speeds_( GroupDependent( energy_groups_, layout_.GenerateSpeedGroups() ) )
{}

// Solve for k eigenvalue
void Slab::EigenvalueSolve()
{
    // Iterate while k is not converged
    while( !KConverged() )
    {
        // Iterate while scalar flux is not converged
        unsigned int i = 0;
        do
        {
            i++;
            UpdateScatterSources();
            ImposeLeftBC();
            SweepRight();
            cells_.back().RightReflectBoundary();
            SweepLeft();
        } while( !ScalarFluxConverged( i ) );
    }
    PrintScalarFluxes();
}

// [Adjoint] Solve for k eigenvalue
void Slab::AdjEigenvalueSolve()
{
    // Iterate while k is not converged
    while( !AdjKConverged() )
    {
        // Iterate while scalar flux is not converged
        unsigned int i = 0;
        do
        {
            i++;
            AdjUpdateScatterSources();
            AdjImposeLeftBC();
            AdjSweepRight();
            cells_.back().AdjRightReflectBoundary();
            AdjSweepLeft();
        } while( !AdjScalarFluxConverged( i ) );
    }
    AdjPrintScalarFluxes();
}

// Solve for fission source matrix
void Slab::FissionMatrixSolve()
{
    std::vector<std::vector<double>> fiss_matrix;
    for( auto j_it = cells_.begin(); j_it != cells_.end(); j_it++ )
    {
        fiss_matrix.push_back( std::vector<double>() );
        std::cout << "Current cell: " << std::distance( cells_.begin(), j_it ) << std::endl;
        // Solve fixed unit source problem with source only in cell j
        std::for_each( cells_.begin(), cells_.end(),
                [this]( Cell &c )
                {
                    c.SetExternalSource( GroupDependent( energy_groups_, 0.0 ) );
                } );
        // External source is divided by cell width to ensure "one" source
        // neutron is produced
        j_it->SetExternalSource( j_it->MaterialReference().FissChi() / j_it->Width() );
        cur_k_ = std::numeric_limits<double>::max();
        FixedSourceSolve();
        // Calculate number of neutrons produced in each cell
        for( auto i_it = cells_.begin(); i_it != cells_.end(); i_it++ )
        {
            fiss_matrix.back().push_back(
                    Dot( i_it->MidpointAngularFluxReference().ScalarFluxReference(),
                        i_it->MaterialReference().FissNu() *
                        i_it->MaterialReference().MacroFissXsec() ) *
                    i_it->Width() );
        }
    }
    // Print fiss_matrix
    std::cout << "#fission_matrix" << std::endl;
    for( auto j_it = fiss_matrix.begin() ; j_it != fiss_matrix.end(); j_it++ )
    {
        for( auto i_it = j_it->begin(); i_it != j_it->end(); i_it++ )
        {
            std::cout << *i_it;
            i_it == prev( j_it->end() ) ? std::cout << std::endl : std::cout << ",";
        }
    }
    std::cout << "#end" << std::endl;
}

// Solve for first generation weighted source (FGWS)
void Slab::FirstGenerationWeightedSourceSolve()
{
    // Solve the forward problem
    EigenvalueSolve();
    adj_cur_k_ = std::numeric_limits<double>::max();
    // Set all cells external source (response) to zero
    std::for_each( cells_.begin(), cells_.end(),
            [this]( Cell &c )
            {
                c.AdjSetExternalSource( GroupDependent( energy_groups_, 0.0 ) );
            } );
    // Solve fixed source problem for each cell
    std::vector<double> result;
    for( auto out_it = cells_.begin(); out_it != cells_.end(); out_it++ )
    {
        std::cout << "Current cell: " << std::distance( cells_.begin(), out_it ) << std::endl;
        // Get response of current cell (fission cross section)
        GroupDependent response = out_it->MaterialReference().FissNu() * out_it->MaterialReference().MacroFissXsec();
        // If response of current cell is zero, the solution is zero
        // everywhere, otherwise solve the fixed source problem
        result.push_back( 0.0 );
        if( response.GroupSum() != 0.0 )
        {
            // Set response of current cell
            out_it->AdjSetExternalSource( response );
            // Solve fixed source problem
            AdjFixedSourceSolve();
            // Calculate inner product of forward k-eigenvalue solution and adjoint
            // fixed source solution
            for( auto in_it = cells_.begin(); in_it != cells_.end(); in_it++ )
            {
                AngularFlux AdjWeightedAngularFlux = in_it->MidpointAngularFluxReference();
                AdjWeightedAngularFlux.WeightBy( in_it->AdjMidpointAngularFluxReference() );
                GroupDependent AdjWeightedScalarFlux = AdjWeightedAngularFlux.ScalarFluxReference() / speeds_;
                result.back() += AdjWeightedScalarFlux.GroupSum();
            }
        }
        // Unset response of current cell to fission cross section
        out_it->AdjSetExternalSource( GroupDependent( energy_groups_, 0.0 ) );
    }
    // Print results
    std::cout << "#first_generation_weighted_source" << std::endl;
    for( auto it = result.begin(); it != result.end(); it++ )
    {
        std::cout << *it;
        it == std::prev( result.end() ) ? std::cout << std::endl : std::cout << ",";
    }
    std::cout << "#end" << std::endl;
}

// Solve for the fission source
void Slab::FissionSourceSolve()
{
    std::cout << "#fission_source" << std::endl;
    for( auto it = cells_.begin(); it != cells_.end(); it++ )
    {
        std::cout << Dot( it->MaterialReference().FissNu() * it->MaterialReference().MacroFissXsec(),
                it->MidpointAngularFluxReference().ScalarFluxReference() );
        it == std::prev( cells_.end() ) ? std::cout << std::endl : std::cout << ",";
    }
    std::cout << "#end" << std::endl;
}

// Solve for fixed source
void Slab::FixedSourceSolve()
{
    unsigned int i = 0;
    do
    {
        i++;
        UpdateScatterSources();
        UpdateFissionSources();
        ImposeLeftBC();
        SweepRight();
        cells_.back().RightReflectBoundary();
        SweepLeft();
    } while( !ScalarFluxConverged( i ) );
}

// [Adjoint] Solve for fixed source
void Slab::AdjFixedSourceSolve()
{
    unsigned int i = 0;
    do
    {
        i++;
        AdjUpdateScatterSources();
        AdjUpdateFissionSources();
        AdjImposeLeftBC();
        AdjSweepRight();
        cells_.back().AdjRightReflectBoundary();
        AdjSweepLeft();
    } while( !AdjScalarFluxConverged( i ) );
}

// Impose left boundary condition
void Slab::ImposeLeftBC()
{
    if( settings_.LeftBC() == Settings::VACUUM )
    {
        cells_.front().LeftVacuumBoundary();
    }
    else if( settings_.LeftBC() == Settings::REFLECTING )
    {
        cells_.front().LeftReflectBoundary();
    }
    else
    {
        assert( false );
    }
}

// [Adjoint] Impose left boundary condition
void Slab::AdjImposeLeftBC()
{
    if( settings_.LeftBC() == Settings::VACUUM )
    {
        cells_.front().AdjLeftVacuumBoundary();
    }
    else if( settings_.LeftBC() == Settings::REFLECTING )
    {
        cells_.front().AdjLeftReflectBoundary();
    }
    else
    {
        assert( false );
    }
}

// Sweep right
void Slab::SweepRight()
{
    for( auto cell_it = std::next( cells_.begin() ); cell_it != cells_.end(); cell_it++ )
    {
        cell_it->SweepRight( std::prev( cell_it )->OutgoingAngularFluxReference() );
    }
}

// [Adjoint] Sweep right
void Slab::AdjSweepRight()
{
    for( auto cell_it = std::next( cells_.begin() ); cell_it != cells_.end(); cell_it++ )
    {
        cell_it->AdjSweepRight( std::prev( cell_it )->AdjOutgoingAngularFluxReference() );
    }
}

// Sweep left
void Slab::SweepLeft()
{
    for( auto cell_it = std::next( cells_.rbegin() ); cell_it != cells_.rend(); cell_it++ )
    {
        cell_it->SweepLeft( std::prev( cell_it )->OutgoingAngularFluxReference() );
    }
}

// [Adjoint] Sweep left
void Slab::AdjSweepLeft()
{
    for( auto cell_it = std::next( cells_.rbegin() ); cell_it != cells_.rend(); cell_it++ )
    {
        cell_it->AdjSweepLeft( std::prev( cell_it )->AdjOutgoingAngularFluxReference() );
    }
}

// Check if k eigenvalue is converged. If not, create new fission source.
bool Slab::KConverged()
{
    // Update the fission source in all cells
    std::for_each( cells_.begin(), cells_.end(),
            []( Cell &c )
            {
                c.UpdateMidpointFissionSource();
            } );

    prev_fission_source_ = cur_fission_source_;
    cur_fission_source_ = std::accumulate( cells_.begin(), cells_.end(), 0.0,
            []( const double &x, Cell &c )
            {
                return x + c.FissionSource();
            } );

    prev_k_ = cur_k_;
    cur_k_ = prev_k_ * cur_fission_source_ / prev_fission_source_;

    double k_error =  std::fabs( ( cur_k_ - prev_k_ ) / prev_k_ );
    std::cout << "k eigenvalue: " << cur_k_ << "\tRelative error: " << k_error << std::endl;

    // Return boolean
    return k_error < settings_.KTol();
}

// [Adjoint] Check if k eigenvalue is converged. If not, create new fission source.
bool Slab::AdjKConverged()
{
    // Update the fission source in all cells
    std::for_each( cells_.begin(), cells_.end(),
            []( Cell &c )
            {
                c.AdjUpdateMidpointFissionSource();
            } );

    adj_prev_fission_source_ = adj_cur_fission_source_;
    adj_cur_fission_source_ = std::accumulate( cells_.begin(), cells_.end(), 0.0,
            []( const double &x, Cell &c )
            {
                return x + c.AdjFissionSource();
            } );

    adj_prev_k_ = adj_cur_k_;
    adj_cur_k_ = adj_prev_k_ * adj_cur_fission_source_ / adj_prev_fission_source_;

    double adj_k_error =  std::fabs( ( adj_cur_k_ - adj_prev_k_ ) / adj_prev_k_ );
    std::cout << "adjoint k eigenvalue: " << adj_cur_k_ << "\tRelative error: " << adj_k_error << std::endl;

    // Return boolean
    return adj_k_error < settings_.KTol();
}

// Check if scalar flux is converged
bool Slab::ScalarFluxConverged( unsigned int iteration )
{
    std::vector<Cell>::iterator max_it = std::max_element( cells_.begin(), cells_.end(),
            []( Cell &smaller, Cell &bigger )
            {
                if( smaller.MaxAbsScalarFluxError() < bigger.MaxAbsScalarFluxError() )
                {
                    return true;
                }
                else
                {
                    return false;
                }
            } );
    double max_abs_rel_error = std::fabs( max_it->MaxAbsScalarFluxError() );
    double sum_sclflux = max_it->MidpointAngularFluxReference().ScalarFluxReference().GroupSum();
    if( iteration % settings_.ProgressPeriod() == 0 )
    {
        std::cout << "Iteration: " << iteration << "\t";
        std::cout << "Relative error: " << max_abs_rel_error << "\t";
        std::cout << "Location cell: " << std::distance( cells_.begin(), max_it ) << "\t";
        std::cout << "Value at cell: " << sum_sclflux << std::endl;
    }

    return max_abs_rel_error < settings_.SclFluxTol();
}

// [Adjoint] Check if scalar flux is converged
bool Slab::AdjScalarFluxConverged( unsigned int iteration )
{
    std::vector<Cell>::iterator max_it = std::max_element( cells_.begin(), cells_.end(),
            []( Cell &smaller, Cell &bigger )
            {
                if( smaller.AdjMaxAbsScalarFluxError() < bigger.AdjMaxAbsScalarFluxError() )
                {
                    return true;
                }
                else
                {
                    return false;
                }
            } );
    double adj_max_abs_rel_error = std::fabs( max_it->AdjMaxAbsScalarFluxError() );
    double adj_sum_sclflux = max_it->AdjMidpointAngularFluxReference().ScalarFluxReference().GroupSum();
    if( iteration % settings_.ProgressPeriod() == 0 )
    {
        std::cout << "Iteration: " << iteration << "\t";
        std::cout << "Relative error: " << adj_max_abs_rel_error << "\t";
        std::cout << "Location cell: " << std::distance( cells_.begin(), max_it ) << "\t";
        std::cout << "Value at cell: " << adj_sum_sclflux << std::endl;
    }

    return adj_max_abs_rel_error < settings_.SclFluxTol();
}

// Calculate new cell scatter sources
void Slab::UpdateScatterSources()
{
    std::for_each( cells_.begin(), cells_.end(),
            []( Cell &c )
            {
                c.UpdateMidpointScatteringSource();
            } );
}

// [Adjoint] Calculate new cell scatter sources
void Slab::AdjUpdateScatterSources()
{
    std::for_each( cells_.begin(), cells_.end(),
            []( Cell &c )
            {
                c.AdjUpdateMidpointScatteringSource();
            } );
}

// Calcualte new cell fission sources
void Slab::UpdateFissionSources()
{
    std::for_each( cells_.begin(), cells_.end(),
            []( Cell &c )
            {
                c.UpdateMidpointFissionSource();
            } );
}

// Calcualte new cell fission sources
void Slab::AdjUpdateFissionSources()
{
    std::for_each( cells_.begin(), cells_.end(),
            []( Cell &c )
            {
                c.AdjUpdateMidpointFissionSource();
            } );
}

// Print scalar fluxes
void Slab::PrintScalarFluxes()
{
    for( auto energy_it = energy_groups_.begin(); energy_it != energy_groups_.end(); energy_it++ )
    {
        std::cout << "#sn_scalar_flux_group_" << *energy_it << "_ev" << std::endl;
        for( auto cell_it = cells_.begin(); cell_it != cells_.end(); cell_it++ )
        {
            std::cout << cell_it->MidpointAngularFluxReference().ScalarFluxReference().at( *energy_it );
            if( cell_it == prev( cells_.end() ) )
            {
                std::cout << std::endl;
            }
            else
            {
                std::cout << ",";
            }
        }
        std::cout << "#end" << std::endl;
    }
}

// [Adjoint] Print scalar fluxes
void Slab::AdjPrintScalarFluxes()
{
    for( auto energy_it = energy_groups_.begin(); energy_it != energy_groups_.end(); energy_it++ )
    {
        std::cout << "#adj_sn_scalar_flux_group_" << *energy_it << "_ev" << std::endl;
        for( auto cell_it = cells_.begin(); cell_it != cells_.end(); cell_it++ )
        {
            std::cout << cell_it->AdjMidpointAngularFluxReference().ScalarFluxReference().at( *energy_it );
            if( cell_it == prev( cells_.end() ) )
            {
                std::cout << std::endl;
            }
            else
            {
                std::cout << ",";
            }
        }
        std::cout << "#end" << std::endl;
    }
}

// Print angular fluxes
void Slab::PrintAngularFluxes()
{
    for( auto energy_it = energy_groups_.begin(); energy_it != energy_groups_.end(); energy_it++ )
    {
        std::cout << "#sn_angular_flux_group_" << *energy_it << "_ev" << std::endl;
        for( auto cell_it = cells_.begin(); cell_it != cells_.end(); cell_it++ )
        {
            std::cout << cell_it->MidpointAngularFluxReference().at( *energy_it );
        }
        std::cout << "#end" << std::endl;
    }
}

// Print angular fluxes
void Slab::AdjPrintAngularFluxes()
{
    for( auto energy_it = energy_groups_.begin(); energy_it != energy_groups_.end(); energy_it++ )
    {
        std::cout << "#adj_sn_angular_flux_group_" << *energy_it << "_ev" << std::endl;
        for( auto cell_it = cells_.begin(); cell_it != cells_.end(); cell_it++ )
        {
            std::cout << cell_it->AdjMidpointAngularFluxReference().at( *energy_it );
        }
        std::cout << "#end" << std::endl;
    }
}

// Print neutron densities
void Slab::PrintNeutronDensities()
{
    for( auto energy_it = energy_groups_.begin(); energy_it != energy_groups_.end(); energy_it++ )
    {
        std::cout << "#sn_neutron_density_group_" << *energy_it << "_ev" << std::endl;
        for( auto cell_it = cells_.begin(); cell_it != cells_.end(); cell_it++ )
        {
            std::cout << cell_it->MidpointAngularFluxReference().ScalarFluxReference().at( *energy_it ) / speeds_.at( *energy_it );
            cell_it == prev( cells_.end() ) ? std::cout << std::endl : std::cout << ",";
        }
        std::cout << "#end" << std::endl;
    }
}

// [Adjoint] Print neutron densities
void Slab::AdjPrintNeutronDensities()
{
    for( auto energy_it = energy_groups_.begin(); energy_it != energy_groups_.end(); energy_it++ )
    {
        std::cout << "#adj_sn_neutron_density_group_" << *energy_it << "_ev" << std::endl;
        for( auto cell_it = cells_.begin(); cell_it != cells_.end(); cell_it++ )
        {
            std::cout << cell_it->AdjMidpointAngularFluxReference().ScalarFluxReference().at( *energy_it ) / speeds_.at( *energy_it );
            cell_it == prev( cells_.end() ) ? std::cout << std::endl : std::cout << ",";
        }
        std::cout << "#end" << std::endl;
    }
}

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Slab &obj )
{
    out << "Settings:\n" << obj.settings_ << std::endl;
    out << "Layout:\n" << obj.layout_ << std::endl;
    for( auto it = obj.cells_.begin(); it != obj.cells_.end(); it++ )
    {
        out << "Cell index: " << it - obj.cells_.begin() << "\t";
        out << *it << std::endl;
    }
    return out;
}
