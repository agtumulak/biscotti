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
AngularFlux::AngularFlux( GroupDependent init_energies, double init_scl_flux ):
    scl_flux_updated_( false )
{
    // Fill angular fluxes
    for( auto energy_it = init_energies.slowest(); energy_it != std::next( init_energies.fastest() ); energy_it++ )
    {
        data_.insert( std::make_pair( energy_it->first, AngleDependent( 0.5 * init_scl_flux ) ) );
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
    scl_flux_updated_ = false;
}

// Reflect boundary (reflecting on right side)
void AngularFlux::RightReflectBoundary()
{
    std::for_each( data_.begin(), data_.end(),
            []( std::pair<const double,AngleDependent> &p )
            {
                p.second.RightReflectBoundary();
            } );
    scl_flux_updated_ = false;
}

// Update scalar flux
void AngularFlux::UpdateScalarFlux()
{
    std::for_each( data_.begin(), data_.end(),
            [&]( std::pair<const double,AngleDependent> &p )
            {
                scl_flux_.Write( p.first, p.second.GetScalarSum() );
            } );
    scl_flux_updated_ = true;
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
