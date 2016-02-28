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
    prev_k_( cur_k_ * settings_.Wrongness() ),
    cur_fission_source_( settings_.FissionSourceGuess() ),
    prev_fission_source_( cur_fission_source_ * settings_.Wrongness() ),
    cells_( layout_.GenerateCells( settings_, cur_k_ ) ),
    energy_groups_( layout_.GenerateEnergyGroups() )
{}

// Solve
void Slab::Solve()
{
    // Enfore vacuum boundary condition
    cells_.front().LeftVacuumBoundary();
    // Iterate while k is not converged
    while( std::fabs( ( cur_k_ - prev_k_ ) / prev_k_ ) > settings_.KTol() )
    {
        // Calculate new sources and k
        UpdateSourceAndK();
        // Iterate while scalar flux is not converged
        while( !ScalarFluxConverged() )
        {
            SweepRight();
            cells_.back().RightReflectBoundary();
            SweepLeft();
            break;
        }
        break;
    }
}

// Print scalar fluxes
void Slab::PrintScalarFluxes() const
{
    for( auto energy_it = energy_groups_.begin(); energy_it != energy_groups_.end(); energy_it++ )
    {
        std::cout << "#sn_scalar_flux_group_" << *energy_it << "_mev" << std::endl;
        for( auto cell_it = cells_.begin(); cell_it != cells_.end(); cell_it++ )
        {
            std::cout << cell_it->ScalarFlux( *energy_it );
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

// Sweep right
void Slab::SweepRight()
{
    for( auto cell_it = std::next( cells_.begin() ); cell_it != cells_.end(); cell_it++ )
    {
        cell_it->SweepRight( std::prev( cell_it )->OutgoingAngularFluxReference() );
    }
}

// Sweep left
void Slab::SweepLeft()
{
    std::cout << "Swept left" << std::endl;
}

// Check if scalar flux is converged
bool Slab::ScalarFluxConverged()
{
    return std::all_of( cells_.begin(), cells_.end(),
            []( const Cell &c )
            {
                return c.IsConverged();
            } );
}

// Calculate new source term for each cell and update K
void Slab::UpdateSourceAndK()
{
    // Store old values
    prev_k_ = cur_k_;
    prev_fission_source_ = cur_fission_source_;

    // Assign new values
    cur_fission_source_ = std::accumulate( cells_.begin(), cells_.end(), 0.0,
            []( const double &x, Cell &c )
            {
            return x + c.UpdateSourcesReturnFission();
            } );
    cur_k_ = cur_fission_source_ / prev_fission_source_;
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
