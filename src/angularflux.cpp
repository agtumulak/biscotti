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

// [Adjoint] Vacuum boundary (outgoing on left side)
void AngularFlux::AdjLeftVacuumBoundary()
{
    std::for_each( data_.begin(), data_.end(),
            []( std::pair<const double,AngleDependent> &p )
            {
                p.second.AdjLeftVacuumBoundary();
            } );
    scl_flux_updated_ = false;
}

// Reflect boundary (reflecting on left side, negative->positive)
void AngularFlux::LeftReflectBoundary()
{
    std::for_each( data_.begin(), data_.end(),
            []( std::pair<const double,AngleDependent> &p )
            {
                p.second.LeftReflectBoundary();
            } );
    scl_flux_updated_ = false;
}

// [Adjoint] Reflect boundary (reflecting on left side, positive->negative)
void AngularFlux::AdjLeftReflectBoundary()
{
    std::for_each( data_.begin(), data_.end(),
            []( std::pair<const double,AngleDependent> &p )
            {
                p.second.AdjLeftReflectBoundary();
            } );
    scl_flux_updated_ = false;
}

// Reflect boundary (reflecting on right side, positive->negative)
void AngularFlux::RightReflectBoundary()
{
    std::for_each( data_.begin(), data_.end(),
            []( std::pair<const double,AngleDependent> &p )
            {
                p.second.RightReflectBoundary();
            } );
    scl_flux_updated_ = false;
}

// [Adjoint] Reflect boundary (reflecting on right side, negative->positive)
void AngularFlux::AdjRightReflectBoundary()
{
    std::for_each( data_.begin(), data_.end(),
            []( std::pair<const double,AngleDependent> &p )
            {
                p.second.AdjRightReflectBoundary();
            } );
    scl_flux_updated_ = false;
}

// Weight all values by another angular flux
void AngularFlux::WeightBy( const AngularFlux &weight )
{
    std::map<double, AngleDependent>::iterator flux_energy_it = slowest();
    std::map<double, AngleDependent>::const_iterator weight_energy_it = weight.slowest();
    for( ; flux_energy_it != std::next( fastest() ); flux_energy_it++, weight_energy_it++ )
    {
        std::map<double,std::pair<double,double>>::iterator flux_angle_it = flux_energy_it->second.neg_begin();
        std::map<double,std::pair<double,double>>::const_iterator weight_angle_it = weight_energy_it->second.neg_begin();
        for( ; flux_angle_it != flux_energy_it->second.pos_end(); flux_angle_it++, weight_angle_it++ )
        {
            flux_angle_it->second.second *= weight_angle_it->second.second;
        }
    }
    scl_flux_updated_ = false;
}

// Update scalar flux
void AngularFlux::UpdateScalarFlux()
{
    std::for_each( data_.begin(), data_.end(),
            [this]( std::pair<const double,AngleDependent> &p )
            {
                scl_flux_.Set( p.first, p.second.WeightedSum() );
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
