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
        double WeightedSum() const;

        // Vacuum boundary (incoming on left side)
        void LeftVacuumBoundary();

        // [Adjoint] Vacuum boundary (outgoing on left side)
        void AdjLeftVacuumBoundary();

        // Reflect boundary (reflecting on left side, negative->positive)
        void LeftReflectBoundary();

        // [Adjoint] Reflect boundary (reflecting on left side, positive->negative)
        void AdjLeftReflectBoundary();

        // Reflect boundary (reflecting on right side, positive->negative)
        void RightReflectBoundary();

        // [Adjoint] Reflect boundary (reflecting on right side, negative->positive)
        void AdjRightReflectBoundary();

        // Iterators //

        // Iterators to positive and negative angles
        typedef std::map<double,std::pair<double,double>>::iterator map_pair_it;
        map_pair_it pos_begin() { return std::prev( data_.end(), data_.size() / 2 ); };
        map_pair_it pos_end() { return data_.end(); };
        map_pair_it neg_begin() { return data_.begin(); };
        map_pair_it neg_end() { return std::next( data_.begin(), data_.size() / 2 ); };

        // Const iterators to positive and negative angles
        typedef std::map<double,std::pair<double,double>>::const_iterator const_map_pair_it;
        const_map_pair_it pos_begin() const { return std::prev( data_.end(), data_.size() / 2 ); };
        const_map_pair_it pos_end() const { return data_.end(); };
        const_map_pair_it neg_begin() const { return data_.begin(); };
        const_map_pair_it neg_end() const { return std::next( data_.begin(), data_.size() / 2 ); };

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
