// cell.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>
#include <vector>

// sn-solver includes
#include "angularflux.hpp"
#include "cell.hpp"

// Default constructor
Cell::Cell( const Settings &settings, const Segment &segment ):
    settings_( settings ),
    segment_( segment ),
    midflux_( AngularFlux( segment_.MaterialReference().TotMacroXsec(), segment_.ScalarFluxGuess() ) ),
    outflux_( AngularFlux( segment_.MaterialReference().TotMacroXsec(), segment_.ScalarFluxGuess() ) )
{}

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Cell &obj )
{
    out << "Cell address: " << &obj << "\t";
    out << "Segment address: " << &obj.segment_ << "\t";
    out << "Settings address: " << &obj.settings_ << "\t";
    return out;
}
