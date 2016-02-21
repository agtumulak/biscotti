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

        // Friend functions //

        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Cell &obj );

    private:

        // Const reference to settings
        const Settings &settings_;

        // Const reference to segment
        const Segment &segment_;

        // Midpoint group angular flux
        AngularFlux midflux_;

        // Outgoing boundary group angular flux
        AngularFlux outflux_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Cell &obj );
