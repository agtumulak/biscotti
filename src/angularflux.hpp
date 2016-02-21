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

    private:

        // Underlying data structure for angular flux
        std::map<double,AngleDependent> data_;
};
