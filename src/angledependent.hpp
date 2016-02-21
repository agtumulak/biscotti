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
        AngleDependent( double init_scl_flux );

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
