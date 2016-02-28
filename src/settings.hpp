// settings.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <map>

class Settings
{
    public:

        // Default constructor
        Settings();

        // Accessors and mutators //

        // Fundamental k eigenvalue guess
        void WriteKGuess( double k_guess ) { k_guess_ = k_guess; };
        double KGuess() const { return k_guess_; };

        // Total fission source guess
        void WriteFissionSourceGuess( double f_guess ) { fission_source_guess_ = f_guess; };
        double FissionSourceGuess() const { return fission_source_guess_; };

        // Fundamental k eigenvalue convergence tolernace
        void WriteKTol( double k_tol ) { k_tol_ = k_tol; };
        double KTol() const { return k_tol_; };

        // Scalar flux convergence tolerance
        void WriteSclFluxTol( double scl_flux_tol ) { scl_flux_tol_ = scl_flux_tol; };
        double SclFluxTol() const { return scl_flux_tol_; };

        // Random number seed
        void WriteSeed( unsigned int seed ) { seed_ = seed; }; 
        unsigned int Seed() const { return seed_; };

        // Period of progress reports
        void WriteProgressPeriod( unsigned int period ) { progress_period_ = period; };
        unsigned int ProgressPeriod() const { return progress_period_; };

        // 'Wrongness' factor to mulitply initial guesses to avoid convergence
        // on first iteration
        void WriteWrongness( double wrongess ) { wrongness_ = wrongess; };
        double Wrongness() const { return wrongness_; };

        // Friend functions //
 
        // Overload I/O operators
        friend std::ostream &operator<< ( std::ostream &out, const Settings &obj );

    private:

        // Fundamental k eigenvalue guess
        double k_guess_;

        // Total fission source guess
        double fission_source_guess_;

        // Fundamental k eigenvalue convergence tolerance
        double k_tol_;

        // Scalar flux convergence tolerance
        double scl_flux_tol_;

        // Random number seed
        unsigned int seed_;

        // Period of progress reports
        unsigned int progress_period_;

        // 'Wrongness' factor to mulitply initial guesses to avoid convergence
        // on first iterations
        double wrongness_;
};

// Friend functions //

// Overload I/O operators
std::ostream &operator<< ( std::ostream &out, const Settings &obj );
