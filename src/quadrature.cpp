// quadrature.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>

// sn-solver includes
#include "quadrature.hpp"

// Default constructor
Quadrature::Quadrature()
{}

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Quadrature &obj )
{
    for( auto it = obj.cosines_and_weights_.begin(); it != obj.cosines_and_weights_.end(); it++ )
    {
        out << "Cosine: " << it->first << "\t" << "Weight: " << it->second << std::endl;
    }
    return out;
}
