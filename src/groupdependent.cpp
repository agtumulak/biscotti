// groupdependent.cpp
// Aaron G. Tumulak

// std includes
#include <cassert>
#include <map>
#include <numeric>
#include <vector>

// sn-solver includes
#include "groupdependent.hpp"

// Default constructor
GroupDependent::GroupDependent()
{}

// Initialize constructor
GroupDependent::GroupDependent( double energy, double value ):
    data_( {{ energy, value }} )
{}

// Element-wise addition of another GroupDependent value
void GroupDependent::GroupDependentAdd( const GroupDependent &g )
{
    std::for_each( data_.begin(), data_.end(),
            [&g]( std::pair<const double,double> &p )
            {
                p.second = p.second + g.at( p.first );
            } );
}

// Multiply each value by a scalar
void GroupDependent::ScalarMultiply( double scalar )
{
    std::for_each( data_.begin(), data_.end(),
            [scalar]( std::pair<const double,double> &p )
            {
                p.second = p.second * scalar;
            } );
}

// Sum all values
double GroupDependent::GroupSum() const
{
    return std::accumulate( data_.begin(), data_.end(), 0.0,
            []( const double &x, const std::pair<double,double> &p )
            { return x + p.second; });
}

// Read value
double GroupDependent::at( double energy ) const
{
    if ( data_.find( energy ) != data_.end() )
    {
        return data_.at( energy );
    }
    else
    {
        return 0.0;
    }
}

// Write value
void GroupDependent::Write( double energy, double value )
{
    // Check input arguments are valid
    assert( energy > 0.0 );

    // Write new value at energy
    data_[ energy ] = value;
}

// Add value
void GroupDependent::Add( double energy, double value )
{
    // Check input arguments are valid
    assert( energy > 0.0 );

    // Add value to energy
    data_[ energy ] += value;
}

// Read energy at index
double GroupDependent::energyat( unsigned int index ) const
{
    // Check that index exists
    assert( index < data_.size() );

    // Return value
    return std::next( data_.begin(), index )->first;
}

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const GroupDependent &obj )
{
    out << std::scientific;

    for(auto it = obj.data_.begin(); it != obj.data_.end(); it++ )
    {
        out << "Group: " << it->first << "\t" << "Value: " << it->second << std::endl;
    }

    out << std::defaultfloat;

    return out;
}

// Overload operator*()
GroupDependent operator* ( const GroupDependent &g, const double &d )
{
    GroupDependent result = g;
    result.ScalarMultiply( d );
    return result;
}
