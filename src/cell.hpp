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
        Cell( const Settings &settings, const Segment &segment );

        // Check if scalar flux is converged
        bool IsConverged() const;

        // Vacuum boundary (incoming on left side)
        void LeftVacuumBoundary() { out_angflux_.LeftVacuumBoundary(); };

        // Reflect boundary (reflecting on right side)
        void RightReflectBoundary() { out_angflux_.RightReflectBoundary(); };
        // Friend functions //

        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Cell &obj );

    private:

        // Const reference to settings
        const Settings &settings_;

        // Const reference to segment
        const Segment &segment_;

        // Midpoint group angular flux
        AngularFlux mid_angflux_;

        // Outgoing boundary group angular flux
        AngularFlux out_angflux_;

        // Previous midpoint scalar flux
        GroupDependent prev_mid_sclflux_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Cell &obj );
