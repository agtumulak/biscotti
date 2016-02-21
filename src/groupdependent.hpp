// groupdependent.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <map>

class GroupDependent
{
        public:

            // Default constructor
            GroupDependent();

            // Initialize constructor
            GroupDependent( double energy, double value );

            // Sum all values
            double GroupSum() const;

            // Accessors and mutators //

            // Read value
            double at( double energy ) const;

            // Write value
            void Write( double energy, double value );

            // Add value
            void Add( double energy, double value );

            // Read energy at index
            double energyat( unsigned int index ) const;

            // Iterators //

            // Const iterators to fastest and slowest group
            std::map<double,double>::const_iterator slowest() const { return data_.begin(); };
            std::map<double,double>::const_iterator fastest() const { return std::prev( data_.end() ); };

            // Friend functions //
            
            // Overload operator<<()
            friend std::ostream &operator<< ( std::ostream &out, const GroupDependent &obj );

        private:

            // Map of energy groups and values
            std::map<double,double> data_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const GroupDependent &obj );
