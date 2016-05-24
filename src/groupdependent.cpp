// groupdependent.cpp
// Aaron G. Tumulak

// std includes
#include <cassert>
#include <cmath>
#include <map>
#include <numeric>
#include <vector>

// biscotti includes
#include "groupdependent.hpp"

// Default constructor
GroupDependent::GroupDependent()
{}

// Initialize constructor
GroupDependent::GroupDependent( double energy, double value ):
    data_( {{ energy, value }} )
{}

// Uniform fill constructor
GroupDependent::GroupDependent( const std::set<double> &energy_groups, const double &value )
{
    std::for_each( energy_groups.begin(), energy_groups.end(),
            [this,&value]( const double &e )
            {
                this->Add( e, value );
            } );
}

// General fill constructor
GroupDependent::GroupDependent( const std::set<double> &energy_groups, const std::set<double> &values )
{
    assert( energy_groups.size() == values.size() );
    std::transform( energy_groups.begin(), energy_groups.end(), values.begin(), std::inserter( data_, data_.end() ),
            []( const double &e, const double &v )
            {
                return std::make_pair( e, v );
            } );
}

// Overload operator+=()
void GroupDependent::operator+= ( const GroupDependent &g )
{
    std::for_each( g.slowest(), std::next( g.fastest() ),
            [this]( const std::pair<const double,double> &p )
            {
                this->Add( p.first, p.second );
            } );
}

// Overload operator*=()
void GroupDependent::operator*=( double scalar )
{
    std::for_each( data_.begin(), data_.end(),
            [scalar]( std::pair<const double,double> &p )
            {
                p.second *= scalar;
            } );
}

// Overload operator/=()
void GroupDependent::operator/= ( double scalar )
{
    std::for_each( data_.begin(), data_.end(),
            [scalar]( std::pair<const double, double> &p )
            {
                p.second /= scalar;
            } );
}

// Sum all values
double GroupDependent::GroupSum() const
{
    return std::accumulate( data_.begin(), data_.end(), 0.0,
            []( const double &x, const std::pair<double,double> &p )
            { return x + p.second; });
}

// Return maximum absolute value
double GroupDependent::MaxAbs() const
{
    typedef std::map<double,double>::const_iterator T;
    T max_it = std::max_element( data_.begin(), data_.end(),
            []( const std::pair<double,double> &smaller, const std::pair<double,double> &bigger )
            {
                if( std::fabs( smaller.second ) < std::fabs( bigger.second ) )
                {
                    return true;
                }
                else
                {
                    return false;
                }
            } );
    return max_it->second;
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

// Set value
void GroupDependent::Set( double energy, double value )
{
    // Check input arguments are valid
    assert( energy > 0.0 );

    // Set new value at energy
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

// Multiply value
void GroupDependent::Multiply( double energy, double value )
{
    // Check input arguments are valid
    assert( energy > 0.0 );

    // Multiply value
    data_[ energy ] *= value;
}

// Divide value
void GroupDependent::Divide( double energy, double value )
{
    // Check input arguments are valid
    assert( energy > 0.0 );

    // Multiply value
    data_[ energy ] /= value;
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

// Overload operator*() (vector scalar product)
GroupDependent operator* ( const GroupDependent &g, const double &d )
{
    GroupDependent result = g;
    result *= d;
    return result;
}

// Overload operator*() (elementwise product)
GroupDependent operator* ( const GroupDependent &u, const GroupDependent &v )
{
    GroupDependent result = u;
    std::for_each( v.slowest(), std::next( v.fastest() ),
            [&result]( const std::pair<double,double> &p )
            {
                result.Multiply( p.first, p.second );
            } );
    return result;
}

// Overload operator/() (vector scalar division)
GroupDependent operator/ ( const GroupDependent &g, const double &d )
{
    GroupDependent result = g;
    result /= d;
    return result;
}

// Overload operator/() (elementwise division)
GroupDependent operator/ ( const GroupDependent &u, const GroupDependent &v )
{
    GroupDependent result = u;
    std::for_each( v.slowest(), std::next( v.fastest() ),
            [&result]( const std::pair<double,double> &p )
            {
                result.Divide( p.first, p.second );
            } );
    return result;
}

// Dot product (vector inner product)
double Dot( const GroupDependent &u, const GroupDependent &v )
{
    return std::inner_product( u.slowest(), std::next( u.fastest() ), v.slowest(), 0.0, std::plus<double>(),
            []( const std::pair<double,double> &p1, const std::pair<double,double> &p2 )
            {
                return p1.second * p2.second;
            } );
}

// Relative error
GroupDependent RelativeError( const GroupDependent &fresh, const GroupDependent &old )
{
    GroupDependent result;
    std::map<double,double>::const_iterator new_it = fresh.slowest();
    std::map<double,double>::const_iterator old_it = old.slowest();
    for( ; new_it != std::next( fresh.fastest() ); new_it++, old_it++ )
    {
        result.Set( new_it->first, ( new_it->second - old_it->second ) / old_it->second );
    }
    return result;
}
