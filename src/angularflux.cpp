// angularflux.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>

// sn-solver includes
#include "angledependent.hpp"
#include "angularflux.hpp"
#include "groupdependent.hpp"

// Default constructor
AngularFlux::AngularFlux( GroupDependent init_energies, double init_scl_flux )
{
    for( auto energy_it = init_energies.slowest(); energy_it != std::next( init_energies.fastest() ); energy_it++ )
    {
        data_.insert( std::make_pair( energy_it->first, AngleDependent( init_scl_flux ) ) );
    }
}
