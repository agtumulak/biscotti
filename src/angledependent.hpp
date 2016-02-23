// angledependent.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <map>

class AngleDependent
{
    public:

        // Default constructor
        AngleDependent( double init_val );

        // Return scalar sum
        double GetScalarSum() const;

        // Vacuum boundary (incoming on left side)
        void LeftVacuumBoundary();

        // Reflect boundary (reflecting on right side)
        void RightReflectBoundary();

        // Iterators //

        typedef std::map<double,std::pair<double,double>>::iterator map_pair_it;
        map_pair_it pos_begin() { return std::prev( data_.end(), data_.size() / 2 ); };
        map_pair_it pos_end() { return data_.end(); };
        map_pair_it neg_begin() { return data_.begin(); };
        map_pair_it neg_end() { return std::next( data_.begin(), data_.size() / 2 ); };

        // Friend functions //

        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const AngleDependent &obj );

    private:

        // Angular flux data
        std::map<double,std::pair<double,double>> data_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const AngleDependent &obj );
