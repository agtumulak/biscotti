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
    out << "Seed: " << obj.seed_ << std::endl;
    out << "Progress report period: " << obj.progress_period_ << std::endl;
    return out;
}
