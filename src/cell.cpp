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
Cell::Cell( const Settings &settings, const Segment &segment, const double &k ):
    settings_( settings ),
    segment_( segment ),
    material_( segment_.MaterialReference() ),
    k_( k ),
    mid_angflux_( AngularFlux( material_.TotMacroXsec(), segment_.ScalarFluxGuess() ) ),
    out_angflux_( AngularFlux( material_.TotMacroXsec(), segment_.ScalarFluxGuess() ) ),
    prev_mid_sclflux_( mid_angflux_.ScalarFluxReference() * 10.0 )
{}

// Return scalar flux at energy
double Cell::ScalarFlux( double energy ) const
{
    return mid_angflux_.ScalarFluxReference().at( energy );
}

// Sweep right
void Cell::SweepRight( const AngularFlux &in_angflux )
{
    // Get cell width
    const double cell_width = segment_.CellWidth();
    // Set up energy iterators
    std::map<double,double>::const_iterator source_it = mid_source_.slowest();
    std::map<double,double>::const_iterator abs_it = material_.TotMacroXsec().slowest();
    std::map<double,AngleDependent>::const_iterator in_energy_it = in_angflux.slowest();
    std::map<double,AngleDependent>::iterator mid_energy_it = mid_angflux_.slowest();
    std::map<double,AngleDependent>::iterator out_energy_it = out_angflux_.slowest();
    // Loop through each energy
    for( ;
            in_energy_it != std::next( in_angflux.fastest() );
            source_it++,
            abs_it++,
            in_energy_it++,
            mid_energy_it++,
            out_energy_it++ )
    {
        // Set up angle iterators
        std::map<double,std::pair<double,double>>::const_iterator in_angle_it = in_energy_it->second.pos_begin();
        std::map<double,std::pair<double,double>>::iterator mid_angle_it = mid_energy_it->second.pos_begin();
        std::map<double,std::pair<double,double>>::iterator out_angle_it = out_energy_it->second.pos_begin();
        // Loop through each angle
        for( ; in_angle_it != in_energy_it->second.pos_end(); in_angle_it++, mid_angle_it++, out_angle_it++ )
        {
            // Write new midpoint angular flux
            mid_angle_it->second.second =
                ( in_angle_it->second.second + 0.5 * cell_width * source_it->second  / in_angle_it->first ) /
                ( 1.0 + 0.5 * abs_it->second * cell_width / in_angle_it->first );
            // Write new outgoing angular flux
            out_angle_it->second.second = 2.0 * mid_angle_it->second.second - in_angle_it->second.second;
        }
    }
    mid_angflux_.UpdateScalarFlux();
    out_angflux_.UpdateScalarFlux();
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

// Vacuum boundary (incoming on left side)
void Cell::LeftVacuumBoundary()
{
    UpdateSourcesReturnFission();
    SweepRight( AngularFlux( material_.TotMacroXsec(), 0.0 ) );
}

// Update source term using current scalar flux and return new fision source
double Cell::UpdateSourcesReturnFission()
{
    double fission_events = material_.MacroFissXsec() * mid_angflux_.ScalarFluxReference() / k_;
    mid_source_ += material_.FissChi() * ( material_.FissNu() * fission_events );
    mid_source_ += material_.MacroScatXsec() * mid_angflux_.ScalarFluxReference();
    return fission_events * segment_.CellWidth();
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
