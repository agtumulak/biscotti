// settings.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>

// biscotti includes
#include "settings.hpp"

// Default constructor
Settings::Settings()
{}

// Friend functions //

// Overload I/O operators
std::ostream &operator<< ( std::ostream &out, const Settings &obj )
{
    out << "K-eff guess: " << obj.k_guess_ << std::endl;
    out << "K-eff convergence tolerance: " << obj.k_tol_ << std::endl;
    out << "Scalar flux convergence tolerance: " << obj.scl_flux_tol_ << std::endl;
    out << "Seed: " << obj.seed_ << std::endl;
    out << "Progress report period: " << obj.progress_period_ << std::endl;
    return out;
}
