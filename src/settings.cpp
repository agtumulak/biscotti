// settings.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>

// sn-solver includes
#include "settings.hpp"

// Default constructor
Settings::Settings()
{}

// Friend functions //

// Overload I/O operators
std::ostream &operator<< ( std::ostream &out, const Settings &obj )
{
    out << "K-eff guess: " << obj.k_guess_ << std::endl;
    out << "Seed: " << obj.seed_ << std::endl;
    out << "Progress report period: " << obj.progress_period_ << std::endl;
    out << "Quadrature:\n\n";
    for( auto it = obj.quadrature_.begin(); it != obj.quadrature_.end(); it++ )
    {
        out << "Angle: " << it->first << "\t" << "Weight: " << it->second << "\n";
    }
    return out;
}
