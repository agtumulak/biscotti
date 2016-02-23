// angularflux.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>

// sn-solver includes
#include "angledependent.hpp"
#include "groupdependent.hpp"

class AngularFlux
{
    public:

        // Default constructor
        AngularFlux( GroupDependent init_energies, double init_scl_flux );

        // Vacuum boundary (incoming on left side)
        void LeftVacuumBoundary();

        // Reflect boundary (reflecting on right side)
        void RightReflectBoundary();

        // Accessors and mutators //

        // Return const reference to scalar flux
        const GroupDependent &ScalarFluxReference() const { return scl_flux_; };

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
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const AngularFlux &obj );
