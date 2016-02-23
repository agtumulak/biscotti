// slab.cpp
// Aaron G. Tumulak

// std includes
#include <algorithm>
#include <cmath>
#include <iostream>
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
    prev_k_( settings_.KGuess() * 10.0 ),
    cur_k_( settings_.KGuess() ),
    cells_( layout_.GenerateCells( settings_ ) ),
    energy_groups_( layout_.GenerateEnergyGroups() )
{
    std::cout << cells_.front() << std::endl;
    cells_.front().LeftVacuumBoundary();
    std::cout << cells_.front() << std::endl;
}

// Solve
void Slab::Solve()
{
    // Enfore vacuum boundary condition
    cells_.front().LeftVacuumBoundary();
    // Iterate while k is not converged
    while( std::fabs( ( cur_k_ - prev_k_ ) / prev_k_ ) > settings_.KTol() )
    {
        // Iterate while scalar flux is not converged
        while( !ScalarFluxConverged() )
        {
            // Loop from highest energy group to slowest energy group
            std::for_each( energy_groups_.rbegin(), energy_groups_.rend(),
                    [&]( double energy )
                    {
                        SweepRight( energy );
                        cells_.back().RightReflectBoundary( energy );
                        SweepLeft( energy );
                    } );
            std::exit( 1 );
        }
    }
}

// Sweep right
void Slab::SweepRight( double energy )
{
    std::cout << "Energy " << energy << " swept right" << std::endl;
}

// Sweep left
void Slab::SweepLeft( double energy )
{
    std::cout << "Energy " << energy << " swept left" << std::endl;
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
