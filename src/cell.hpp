// cell.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <map>

// sn-solver includes
#include "angularflux.hpp"
#include "segment.hpp"
#include "settings.hpp"

class Cell
{
    public:

        // Default constructor
        Cell( const Settings &settings, const Segment &segment, const double &k );

        // Return scalar flux at energy
        double ScalarFlux( double energy ) const;

        // Sweep right
        void SweepRight( const AngularFlux &in_angflux );

        // Check if scalar flux is converged
        bool IsConverged() const;

        // Vacuum boundary (incoming on left side)
        void LeftVacuumBoundary();

        // Reflect boundary (reflecting on right side)
        void RightReflectBoundary() { out_angflux_.RightReflectBoundary(); };

        // Update source term using current scalar flux and return new fision source
        double UpdateSourcesReturnFission();

        // Accessors and mutators //

        // Const reference to outgoing angular flux
        const AngularFlux &OutgoingAngularFluxReference() const { return out_angflux_; };

        // Friend functions //

        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Cell &obj );

    private:

        // Const reference to settings
        const Settings &settings_;

        // Const reference to segment
        const Segment &segment_;

        // Const reference to material
        const Material &material_;

        // Const reference to k eigenvalue
        const double &k_;

        // Midpoint group angular flux
        AngularFlux mid_angflux_;

        // Outgoing boundary group angular flux
        AngularFlux out_angflux_;

        // Previous midpoint scalar flux
        GroupDependent prev_mid_sclflux_;

        // Midpoint isotropic source term
        GroupDependent mid_source_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Cell &obj );
