// settings.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <map>

// sn-solver includes
#include "quadrature.hpp"

class Settings
{
    public:

        // Default constructor
        Settings();

        // Accessors and mutators //

        // Fundamental k eigenvalue guess
        void WriteKGuess( double k_guess ) { k_guess_ = k_guess; };
        double KGuess() { return k_guess_; };

        // Random number seed
        void WriteSeed( unsigned int seed ) { seed_ = seed; }; 
        unsigned int Seed() const { return seed_; };

        // Period of progress reports
        void WriteProgressPeriod( unsigned int period ) { progress_period_ = period; };
        unsigned int ProgressPeriod() const { return progress_period_; };

        // Quadrature to use for ordinates
        void WriteQuadrature( Quadrature quadrature ) { quadrature_ = quadrature; };
        const Quadrature &QuadratureReference() const { return quadrature_; };

        // Friend functions //
 
        // Overload I/O operators
        friend std::ostream &operator<< ( std::ostream &out, const Settings &obj );

    private:

        // Fundamental k eigenvalue guess
        double k_guess_;

        // Random number seed
        unsigned int seed_;

        // Period of progress reports
        unsigned int progress_period_;

        // Quadrature to use for ordinates
        Quadrature quadrature_;
};

// Friend functions //

// Overload I/O operators
std::ostream &operator<< ( std::ostream &out, const Settings &obj );
