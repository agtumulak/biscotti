// angularflux.cpp
// Aaron G. Tumulak

// std includes
#include <algorithm>
#include <iostream>

// sn-solver includes
#include "angledependent.hpp"
#include "angularflux.hpp"
#include "groupdependent.hpp"

// Default constructor
AngularFlux::AngularFlux( GroupDependent init_energies, double init_scl_flux )
{
    // Fill angular fluxes and scalar fluxes
    for( auto energy_it = init_energies.slowest(); energy_it != std::next( init_energies.fastest() ); energy_it++ )
    {
        data_.insert( std::make_pair( energy_it->first, AngleDependent( 0.5 * init_scl_flux ) ) );
        scl_flux_.Write( energy_it->first, init_scl_flux );
    }
}

// Vacuum boundary (incoming on left side)
void AngularFlux::LeftVacuumBoundary()
{
    std::for_each( data_.begin(), data_.end(),
            []( std::pair<const double,AngleDependent> &p )
            {
                p.second.LeftVacuumBoundary();
            } );
    UpdateScalarFlux();
}

// Reflect boundary (reflecting on right side)
void AngularFlux::RightReflectBoundary( double energy )
{
    UpdateScalarFlux();
}

// Update scalar flux
void AngularFlux::UpdateScalarFlux()
{
    std::for_each( data_.begin(), data_.end(),
            [&]( std::pair<const double,AngleDependent> &p )
            {
                scl_flux_.Write( p.first, p.second.GetScalarSum() );
            } );
}

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const AngularFlux &obj )
{
    for( auto it = obj.data_.begin(); it != obj.data_.end(); it++ )
    {
        out << "Energy group: " << it->first << std::endl;
        out << it->second << std::endl;
    }
    return out;
}
