// angledependent.cpp
// Aaron G. Tumulak

// std includes
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <numeric>

// sn-solver includes
#include "angledependent.hpp"

// Default constructor
AngleDependent::AngleDependent( double init_val )
{
    std::initializer_list<std::initializer_list<double>> il;
    #include "quadrature.dsv"
    for( auto it = il.begin(); it != il.end(); it++ )
    {
        assert( it->size() == 2 );
        data_[ *std::next( it->begin() ) ] = std::make_pair( *it->begin() , init_val );
    }
}

// Return scalar sum
double AngleDependent::WeightedSum() const
{
    return std::accumulate( data_.begin(), data_.end(), 0.0,
            []( const double &x, const std::pair<double,std::pair<double,double>> &p )
            {
                return x + p.second.first * p.second.second;
            } );
}

// Vacuum boundary (incoming on left side)
void AngleDependent::LeftVacuumBoundary()
{
    std::for_each( pos_begin(), pos_end(),
            []( std::pair<const double,std::pair<double,double>> &p )
            {
                p.second.second = 0.0;
            } );
}

// [Adjoint] Vacuum boundary (outgoing on left side)
void AngleDependent::AdjLeftVacuumBoundary()
{
    std::for_each( neg_begin(), neg_end(),
            []( std::pair<const double,std::pair<double,double>> &p )
            {
                p.second.second = 0.0;
            } );
}

// Reflect boundary (reflecting on right side)
void AngleDependent::RightReflectBoundary()
{
    typedef std::map<double,std::pair<double,double>>::iterator angflux_it;
    typedef std::map<double,std::pair<double,double>>::const_reverse_iterator const_reverse_angflux_it;
    angflux_it neg_it = data_.begin(); // Negative iterator moving positive
    const_reverse_angflux_it pos_it = data_.rbegin(); // Positive iterator moving negative
    for( ; neg_it != std::next( data_.begin(), data_.size() / 2 ); neg_it++, pos_it++ )
    {
        neg_it->second.second = pos_it->second.second;
    }
}

// [Adjoint] Reflect boundary (reflecting on right side, negative->positive)
void AngleDependent::AdjRightReflectBoundary()
{
    typedef std::map<double,std::pair<double,double>>::const_iterator const_angflux_it;
    typedef std::map<double,std::pair<double,double>>::reverse_iterator reverse_angflux_it;
    const_angflux_it neg_it = data_.begin(); // Negative iterator moving positive
    reverse_angflux_it pos_it = data_.rbegin(); // Positive iterator moving negative
    for( ; neg_it != std::next( data_.begin(), data_.size() / 2 ); neg_it++, pos_it++ )
    {
        pos_it->second.second = neg_it->second.second;
    }
}

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const AngleDependent &obj )
{
    for( auto it = obj.data_.begin(); it != obj.data_.end(); it++ )
    {
        out << it->second.second;
        if( it == prev( obj.data_.end() ) )
        {}
        else
        {
            std::cout << ",";
        }
    }
    return out;
}
