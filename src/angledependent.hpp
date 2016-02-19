// angledependent.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <map>

// sn-solver includes
#include "quadrature.hpp"

class AngleDependent
{
    public:

        // Default constructor
        AngleDependent( const Quadrature &quadrature, double init_scl_flux );

    private:

        // Angular flux data
        std::map<double,std::pair<double,double>>  data_;
};
