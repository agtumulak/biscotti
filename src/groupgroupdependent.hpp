// groupgroupdependent.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <map>

// biscotti includes
#include "groupdependent.hpp"

class GroupGroupDependent
{
        public:

            // Default constructor
            GroupGroupDependent();

            // Accessors and mutators //

            // Read value
            GroupDependent at( double energy ) const;

            // Set value
            void Set( double energy_1, double energy_2, double value );

            // Set energy group
            void SetGroup( double energy, const GroupDependent &value );

            // Iterators //

            // Const iterators to fastest and slowest group
            std::map<double,GroupDependent>::const_iterator slowest() const { return data_.begin(); };
            std::map<double,GroupDependent>::const_iterator fastest() const { return std::prev( data_.end() ); };

            // Friend functions //

            // Overload operator<<()
            friend std::ostream &operator<< ( std::ostream &out, const GroupGroupDependent &obj );

            // Overload operator*() (matrix-vector multiplication)
            friend GroupDependent operator* ( const GroupGroupDependent &m, const GroupDependent &v );

        private:

            // Map typedef
            typedef std::map<double,GroupDependent> groupgroupmap;

            // Map of energy groups and values
            groupgroupmap data_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const GroupGroupDependent &obj );

// Overload operator*() (matrix-vector multiplication)
GroupDependent operator* ( const GroupGroupDependent &m, const GroupDependent &v );
