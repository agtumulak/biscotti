// angledependent.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>
#include <map>

// sn-solver includes
#include "angledependent.hpp"
#include "quadrature.hpp"

// Default constructor
AngleDependent::AngleDependent( const Quadrature &quadrature, double init_scl_flux )
{
    for( auto it = quadrature.begin(); it != quadrature.end(); it++ )
    {
        data_[ it->first ] = std::make_pair( it->second, 0.5 * init_scl_flux );
    }
}
