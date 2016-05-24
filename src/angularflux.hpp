// angularflux.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>

// biscotti includes
#include "angledependent.hpp"
#include "groupdependent.hpp"

class AngularFlux
{
    public:

        // Default constructor
        AngularFlux( GroupDependent init_energies, double init_scl_flux );

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

        // Weight all values by another angular flux
        void WeightBy( const AngularFlux &weight );

        // Accessors and mutators //

        // Return const reference to AngleDependent value at energy
        const AngleDependent &at( double energy ) const { return data_.at( energy ); };

        // Return const reference to scalar flux
        const GroupDependent &ScalarFluxReference()
        {
            if( !scl_flux_updated_ )
            {
                UpdateScalarFlux();
            }
            return scl_flux_;
        };

        // Iterators //

        // Iterators to fastest and slowest group
        std::map<double,AngleDependent>::iterator slowest() { scl_flux_updated_ = false; return data_.begin(); };
        std::map<double,AngleDependent>::iterator fastest() { scl_flux_updated_ = false; return std::prev( data_.end() ); };

        // Const iterators to fastest and slowest group
        std::map<double,AngleDependent>::const_iterator slowest() const { return data_.begin(); };
        std::map<double,AngleDependent>::const_iterator fastest() const { return std::prev( data_.end() ); };

        // Friend functions //

        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const AngularFlux &obj );

    private:

        // Update scalar flux
        void UpdateScalarFlux();

        // Underlying data structure for angular flux
        std::map<double,AngleDependent> data_;

        // Scalar flux
        GroupDependent scl_flux_;

        // Scalar flux is updated
        bool scl_flux_updated_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const AngularFlux &obj );
