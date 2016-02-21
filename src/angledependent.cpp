// angledependent.cpp
// Aaron G. Tumulak

// std includes
#include <cassert>
#include <iostream>
#include <map>

// sn-solver includes
#include "angledependent.hpp"

// Default constructor
AngleDependent::AngleDependent( double init_scl_flux )
{
    std::initializer_list<std::initializer_list<double>> il;
    #include "quadrature.dsv"
    for( auto it = il.begin(); it != il.end(); it++ )
    {
        assert( it->size() == 2 );
        data_[ *std::next( it->begin() ) ] = std::make_pair( *it->begin() , 0.5 * init_scl_flux );
    }
}

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const AngleDependent &obj )
{
    for( auto it = obj.data_.begin(); it != obj.data_.end(); it++ )
    {
        out << "Cosine: " << it->first << "\t";
        out << "Weight: " << it->second.first << "\t";
        out << "Value: " << it->second.second << std::endl;
    }
    return out;
}
