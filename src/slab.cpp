// slab.cpp
// Aaron G. Tumulak

// std includes
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

// sn-solver includes
#include "cell.hpp"
#include "layout.hpp"
#include "settings.hpp"
#include "slab.hpp"

// Default constructor
Slab::Slab( const Settings &settings, const Layout &layout ):
    settings_( settings ),
    layout_( layout ),
    cur_k_( settings_.KGuess() ),
    cur_fission_source_( settings_.FissionSourceGuess() ),
    cells_( layout_.GenerateCells( settings_, cur_k_ ) ),
    energy_groups_( layout_.GenerateEnergyGroups() )
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
            cells_.front().LeftVacuumBoundary();
            SweepRight();
            cells_.back().RightReflectBoundary();
            SweepLeft();
        } while( !ScalarFluxConverged( i ) );
    }
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
            cells_.front().AdjLeftVacuumBoundary();
            AdjSweepRight();
            cells_.back().AdjRightReflectBoundary();
            AdjSweepLeft();
        } while( !ScalarFluxConverged( i ) );
    }
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
        cells_.front().LeftVacuumBoundary();
        SweepRight();
        cells_.back().RightReflectBoundary();
        SweepLeft();
    } while( !ScalarFluxConverged( i ) );
}

// Print scalar fluxes
void Slab::PrintScalarFluxes()
{
    for( auto energy_it = energy_groups_.begin(); energy_it != energy_groups_.end(); energy_it++ )
    {
        std::cout << "#sn_scalar_flux_group_" << *energy_it << "_mev" << std::endl;
        for( auto cell_it = cells_.begin(); cell_it != cells_.end(); cell_it++ )
        {
            std::cout << cell_it->MidpointScalarFlux( *energy_it );
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
        std::cout << "#sn_angular_flux_group_" << *energy_it << "_mev" << std::endl;
        for( auto cell_it = cells_.begin(); cell_it != cells_.end(); cell_it++ )
        {
            std::cout << cell_it->MidpointAngularFlux( *energy_it ) << std::endl;
        }
        std::cout << "#end" << std::endl;
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
        cell_it->AdjSweepRight( std::prev( cell_it )->OutgoingAngularFluxReference() );
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
        cell_it->AdjSweepLeft( std::prev( cell_it )->OutgoingAngularFluxReference() );
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

    prev_fission_source_ = cur_fission_source_;
    cur_fission_source_ = std::accumulate( cells_.begin(), cells_.end(), 0.0,
            []( const double &x, Cell &c )
            {
                return x + c.FissionSource();
            } );

    prev_k_ = cur_k_;
    cur_k_ = prev_k_ * cur_fission_source_ / prev_fission_source_;

    double k_error =  std::fabs( ( cur_k_ - prev_k_ ) / prev_k_ );
    std::cout << "adjoint k eigenvalue: " << cur_k_ << "\tRelative error: " << k_error << std::endl;

    // Return boolean
    return k_error < settings_.KTol();
}

// Check if scalar flux is converged
bool Slab::ScalarFluxConverged( unsigned int i )
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
    if( i % settings_.ProgressPeriod() == 0 )
    {
        std::cout << "Iteration: " << i << "\t";
        std::cout << "Relative error: " << max_abs_rel_error << "\t";
        std::cout << "Location cell: " << std::distance( cells_.begin(), max_it ) << std::endl;
    }

    return max_abs_rel_error < settings_.SclFluxTol();
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
