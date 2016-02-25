// groupgroupdependent.cpp
// Aaron G. Tumulak

// std includes
#include <cassert>
#include <vector>

// sn-solver includes
#include "groupgroupdependent.hpp"

// Default constructor
GroupGroupDependent::GroupGroupDependent()
{}

// Read value
GroupDependent GroupGroupDependent::at( double energy ) const
{
    // Check that group exists
    assert( data_.find( energy ) != data_.end() );

    // Return value
    return data_.at( energy );
}

// Write value
void GroupGroupDependent::Write( double energy_1, double energy_2, double value )
{
    // Check input arguments are valid
    assert( energy_1 > 0.0 && energy_1 > 0.0 && value >= 0.0 );

    // Check if GroupDependent value exists at energy_1
    if( data_.find( energy_1 ) != data_.end() )
    {
        // Group at energy_1 exists, write/overwrite value at energy_2
        data_[ energy_1 ].Write( energy_2, value );
    }
    else
    {
        // Group at energy_1 doesn't exist, create new group
        data_[ energy_1 ] = GroupDependent( energy_2, value );
    }
}

// Write energy group
void GroupGroupDependent::WriteGroup( double energy, const GroupDependent &value )
{
    // Check input arguments are valid
    assert( energy > 0.0 );

    // Add value to data
    data_[ energy ] = value;
}

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const GroupGroupDependent &obj )
{
    for( auto it = obj.data_.begin(); it != obj.data_.end(); it++ )
    {
        out << "Group: " << std::scientific << it->first << std::endl;
        out << it->second;
        if( it != prev( obj.data_.end() ) )
        {
            out << std::endl;
        }
    }

    out << std::defaultfloat;

    return out;
}

// Overload operator*() (matrix-vector multiplication)
GroupDependent operator* ( const GroupGroupDependent &m, const GroupDependent &v )
{
    GroupDependent result;
    // Loop through each groupdependent object (an outscatter vector)
    auto m_it = m.slowest();
    auto v_it = v.slowest();
    for( ; m_it != m.fastest(); m_it++, v_it++ )
    {
        // Contribution to group dependent scalar flux from scalar flux at
        // given energy v_it->first
        result.GroupDependentAdd( m_it->second * v_it->second );
    }
    return result;
}

GroupGroupDependent operator* ( const GroupDependent &u, const GroupDependent &v )
{
    GroupGroupDependent result;
    for( auto it = v.slowest(); it != v.fastest(); it++ )
    {
        result.WriteGroup( it->first, u * it->second );
    }
    return result;
}
