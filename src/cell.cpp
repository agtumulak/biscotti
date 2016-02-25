// cell.cpp
// Aaron G. Tumulak

// std includes
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

// sn-solver includes
#include "angularflux.hpp"
#include "cell.hpp"

// Default constructor
Cell::Cell( const Settings &settings, const Segment &segment ):
    settings_( settings ),
    segment_( segment ),
    mid_angflux_( AngularFlux( segment_.MaterialReference().TotMacroXsec(), segment_.ScalarFluxGuess() ) ),
    out_angflux_( AngularFlux( segment_.MaterialReference().TotMacroXsec(), segment_.ScalarFluxGuess() ) ),
    prev_mid_sclflux_( mid_angflux_.ScalarFluxReference() * 10.0 )
{}

// Sweep right
void Cell::SweepRight( const AngularFlux &incoming_flux )
{
    std::cout << "Swept right!" << std::endl;
}

// Check if scalar flux is converged
bool Cell::IsConverged() const
{
    typedef std::map<double,double>::const_iterator map_it;
    map_it cur_sclflux_it = mid_angflux_.ScalarFluxReference().slowest();
    map_it prev_sclflux_it = prev_mid_sclflux_.slowest();
    // Check if midpoint scalar fluxes are converged
    for( ; cur_sclflux_it != std::next( mid_angflux_.ScalarFluxReference().fastest() ); cur_sclflux_it++, prev_sclflux_it++ )
    {
        double rel_err = std::fabs( ( cur_sclflux_it->second - prev_sclflux_it->second ) / prev_sclflux_it->second );
        if( rel_err < settings_.SclFluxTol() )
        {
            return true;
        }
    }
    return false;
}

// Get source term using previous iteration scalar flux
GroupDependent Cell::PrevIterationSource() const
{
    GroupDependent mid_source;

    // Scattering source //
    mid_source.GroupDependentAdd(
            segment_.MaterialReference().MacroScatXsec() *
            prev_mid_sclflux_ );

    // Fission source //
    mid_source.GroupDependentAdd(
            segment_.MaterialReference().FissChi() *
            segment_.MaterialReference().FissNu() *
            segment_.MaterialReference().MacroFissXsec() *
            prev_mid_sclflux_ );

    return mid_source;
}

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Cell &obj )
{
    out << "Cell address: " << &obj << "\t";
    out << "Segment address: " << &obj.segment_ << "\t";
    out << "Settings address: " << &obj.settings_ << "\t";
    std::cout << std::endl;
    std::cout << "Midpoint flux:\n\n" << obj.mid_angflux_ << std::endl;
    std::cout << "Outgoing flux:\n\n" << obj.out_angflux_ << std::endl;
    std::cout << "Scalar flux:\n\n" << obj.prev_mid_sclflux_ << std::endl;
    return out;
}
