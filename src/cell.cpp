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
double Cell::MidpointScalarFlux( double energy )
{
    return mid_angflux_.ScalarFluxReference().at( energy );
}

// Return angular flux at energy
const AngleDependent &Cell::MidpointAngularFlux( double energy ) const
{
    return mid_angflux_.AngularFluxReference().at( energy );
}

// Sweep right
void Cell::SweepRight( const AngularFlux &in_angflux )
{
    prev_mid_sclflux_ = mid_angflux_.ScalarFluxReference();
    // Set up energy iterators
    std::map<double,double>::const_iterator ext_src_it = material_.ExtSource().slowest();
    std::map<double,double>::const_iterator fiss_src_it = mid_fiss_src_.slowest();
    std::map<double,double>::const_iterator scat_src_it = mid_scat_src_.slowest();
    std::map<double,double>::const_iterator abs_it = material_.TotMacroXsec().slowest();
    std::map<double,AngleDependent>::const_iterator in_energy_it = in_angflux.slowest();
    std::map<double,AngleDependent>::iterator mid_energy_it = mid_angflux_.slowest();
    std::map<double,AngleDependent>::iterator out_energy_it = out_angflux_.slowest();
    // Loop through each energy
    for( ;
            in_energy_it != std::next( in_angflux.fastest() );
            ext_src_it++,
            fiss_src_it++,
            scat_src_it++,
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
                ( in_angle_it->second.second + 0.25 * segment_.CellWidth() *
                  ( ext_src_it->second + fiss_src_it->second + scat_src_it->second ) /
                  in_angle_it->first ) /
                ( 1.0 + 0.5 * abs_it->second * segment_.CellWidth() / in_angle_it->first );
            // Write new outgoing angular flux
            out_angle_it->second.second = 2.0 * mid_angle_it->second.second - in_angle_it->second.second;
        }
    }
}

// [Adjoint] Sweep right
void Cell::AdjSweepRight( const AngularFlux &in_angflux )
{
    prev_mid_sclflux_ = mid_angflux_.ScalarFluxReference();
    // Set up energy iterators
    std::map<double,double>::const_iterator ext_src_it = material_.ExtSource().slowest();
    std::map<double,double>::const_iterator fiss_src_it = mid_fiss_src_.slowest();
    std::map<double,double>::const_iterator scat_src_it = mid_scat_src_.slowest();
    std::map<double,double>::const_iterator abs_it = material_.TotMacroXsec().slowest();
    std::map<double,AngleDependent>::const_iterator in_energy_it = in_angflux.slowest();
    std::map<double,AngleDependent>::iterator mid_energy_it = mid_angflux_.slowest();
    std::map<double,AngleDependent>::iterator out_energy_it = out_angflux_.slowest();
    // Loop through each energy
    for( ;
            in_energy_it != std::next( in_angflux.fastest() );
            ext_src_it++,
            fiss_src_it++,
            scat_src_it++,
            abs_it++,
            in_energy_it++,
            mid_energy_it++,
            out_energy_it++ )
    {
        // Set up angle iterators
        std::map<double,std::pair<double,double>>::const_iterator in_angle_it = in_energy_it->second.neg_begin();
        std::map<double,std::pair<double,double>>::iterator mid_angle_it = mid_energy_it->second.neg_begin();
        std::map<double,std::pair<double,double>>::iterator out_angle_it = out_energy_it->second.neg_begin();
        // Loop through each angle
        for( ; in_angle_it != in_energy_it->second.neg_end(); in_angle_it++, mid_angle_it++, out_angle_it++ )
        {
            // Write new midpoint angular flux
            mid_angle_it->second.second =
                ( in_angle_it->second.second - 0.25 * segment_.CellWidth() *
                  ( ext_src_it->second + fiss_src_it->second + scat_src_it->second ) /
                  in_angle_it->first ) /
                ( 1.0 - 0.5 * abs_it->second * segment_.CellWidth() / in_angle_it->first );
            // Write new outgoing angular flux
            out_angle_it->second.second = 2.0 * mid_angle_it->second.second - in_angle_it->second.second;
        }
    }
}

// Sweep left
void Cell::SweepLeft( const AngularFlux &in_angflux )
{
    prev_mid_sclflux_ = mid_angflux_.ScalarFluxReference();
    // Set up energy iterators
    std::map<double,double>::const_iterator ext_src_it = material_.ExtSource().slowest();
    std::map<double,double>::const_iterator fiss_src_it = mid_fiss_src_.slowest();
    std::map<double,double>::const_iterator scat_src_it = mid_scat_src_.slowest();
    std::map<double,double>::const_iterator abs_it = material_.TotMacroXsec().slowest();
    std::map<double,AngleDependent>::const_iterator in_energy_it = in_angflux.slowest();
    std::map<double,AngleDependent>::iterator mid_energy_it = mid_angflux_.slowest();
    std::map<double,AngleDependent>::iterator out_energy_it = out_angflux_.slowest();
    // Loop through each energy
    for( ;
            in_energy_it != std::next( in_angflux.fastest() );
            ext_src_it++,
            fiss_src_it++,
            scat_src_it++,
            abs_it++,
            in_energy_it++,
            mid_energy_it++,
            out_energy_it++ )
    {
        // Set up angle iterators
        std::map<double,std::pair<double,double>>::const_iterator in_angle_it = in_energy_it->second.neg_begin();
        std::map<double,std::pair<double,double>>::iterator mid_angle_it = mid_energy_it->second.neg_begin();
        std::map<double,std::pair<double,double>>::iterator out_angle_it = out_energy_it->second.neg_begin();
        // Loop through each angle
        for( ; in_angle_it != in_energy_it->second.neg_end(); in_angle_it++, mid_angle_it++, out_angle_it++ )
        {
            // Write new midpoint angular flux
            mid_angle_it->second.second =
                ( in_angle_it->second.second - 0.25 * segment_.CellWidth() *
                  ( ext_src_it->second + fiss_src_it->second + scat_src_it->second ) /
                  in_angle_it->first ) /
                ( 1.0 - 0.5 * abs_it->second * segment_.CellWidth() / in_angle_it->first );
            // Write new outgoing angular flux
            out_angle_it->second.second = 2.0 * mid_angle_it->second.second - in_angle_it->second.second;
        }
    }
}

// [Adjoint] Sweep left
void Cell::AdjSweepLeft( const AngularFlux &in_angflux )
{
    prev_mid_sclflux_ = mid_angflux_.ScalarFluxReference();
    // Set up energy iterators
    std::map<double,double>::const_iterator ext_src_it = material_.ExtSource().slowest();
    std::map<double,double>::const_iterator fiss_src_it = mid_fiss_src_.slowest();
    std::map<double,double>::const_iterator scat_src_it = mid_scat_src_.slowest();
    std::map<double,double>::const_iterator abs_it = material_.TotMacroXsec().slowest();
    std::map<double,AngleDependent>::const_iterator in_energy_it = in_angflux.slowest();
    std::map<double,AngleDependent>::iterator mid_energy_it = mid_angflux_.slowest();
    std::map<double,AngleDependent>::iterator out_energy_it = out_angflux_.slowest();
    // Loop through each energy
    for( ;
            in_energy_it != std::next( in_angflux.fastest() );
            ext_src_it++,
            fiss_src_it++,
            scat_src_it++,
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
                ( in_angle_it->second.second + 0.25 * segment_.CellWidth() *
                  ( ext_src_it->second + fiss_src_it->second + scat_src_it->second ) /
                  in_angle_it->first ) /
                ( 1.0 + 0.5 * abs_it->second * segment_.CellWidth() / in_angle_it->first );
            // Write new outgoing angular flux
            out_angle_it->second.second = 2.0 * mid_angle_it->second.second - in_angle_it->second.second;
        }
    }
}

// Vacuum boundary (incoming on left side)
void Cell::LeftVacuumBoundary()
{
    // Create angular flux at boundary
    AngularFlux in_angflux( material_.TotMacroXsec(), 0.0 );
    SweepRight( in_angflux );
}

// [Adjoint] Vacuum boundary (outgoing on left side)
void Cell::AdjLeftVacuumBoundary()
{
    // Create angular flux at boundary
    AngularFlux in_angflux( material_.TotMacroXsec(), 0.0 );
    AdjSweepRight( in_angflux );
}

// Reflect boundary (reflecting on right side, positive->negative)
void Cell::RightReflectBoundary()
{
    // Create angular flux at boundary
    AngularFlux in_angflux = out_angflux_;
    in_angflux.RightReflectBoundary();
    SweepLeft( in_angflux );
}

// [Adjoint] Reflect boundary (reflecting on right side, negative->positive)
void Cell::AdjRightReflectBoundary()
{
    // Create angular flux at boundary
    AngularFlux in_angflux = out_angflux_;
    in_angflux.AdjRightReflectBoundary();
    AdjSweepLeft( in_angflux );
}

// Return scalar flux error
double Cell::MaxAbsScalarFluxError()
{
    GroupDependent rel_errors = RelativeError( mid_angflux_.ScalarFluxReference(),  prev_mid_sclflux_ );
    return rel_errors.MaxAbs();
}

// Update midpoint scattering source term
void Cell::UpdateMidpointScatteringSource()
{
    mid_scat_src_ = material_.MacroScatXsec() * mid_angflux_.ScalarFluxReference();
}

// [Adjoint] Update midpoint scattering source term
void Cell::AdjUpdateMidpointScatteringSource()
{
    mid_scat_src_ = material_.AdjMacroScatXsec() * mid_angflux_.ScalarFluxReference();
}

void Cell::UpdateMidpointFissionSource()
{
    double fission_rate = Dot( material_.FissNu() * material_.MacroFissXsec(),  mid_angflux_.ScalarFluxReference() ) / k_;
    mid_fiss_src_ = material_.FissChi() * fission_rate;
}

void Cell::AdjUpdateMidpointFissionSource()
{
    double fission_rate = Dot( material_.FissChi(),  mid_angflux_.ScalarFluxReference() ) / k_;
    mid_fiss_src_ = material_.FissNu() * material_.MacroFissXsec() * fission_rate;
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
