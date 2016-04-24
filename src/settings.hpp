// settings.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <map>

class Settings
{
    public:

        // Enumerate boundary condition types
        enum BoundaryCondition
        {
            VACUUM,
            REFLECTING
        };

        // Default constructor
        Settings();

        // Accessors and mutators //

        // Left boundary condition
        void SetLeftBC( BoundaryCondition bc ) { left_bc_ = bc; };
        BoundaryCondition LeftBC() const { return left_bc_; };

        // Fundamental k eigenvalue guess
        void SetKGuess( double k_guess ) { k_guess_ = k_guess; };
        double KGuess() const { return k_guess_; };

        // [Adjoint] Fundamental k eigenvalue guess
        void AdjSetKGuess( double adj_k_guess ) { adj_k_guess_ = adj_k_guess; };
        double AdjKGuess() const { return adj_k_guess_; };

        // Total fission source guess
        void SetFissionSourceGuess( double f_guess ) { fission_source_guess_ = f_guess; };
        double FissionSourceGuess() const { return fission_source_guess_; };

        // [Adjoint] Total fission source guess
        void AdjSetFissionSourceGuess( double adj_f_guess ) { adj_fission_source_guess_ = adj_f_guess; };
        double AdjFissionSourceGuess() const { return adj_fission_source_guess_; };

        // Fundamental k eigenvalue convergence tolernace
        void SetKTol( double k_tol ) { k_tol_ = k_tol; };
        double KTol() const { return k_tol_; };

        // Scalar flux convergence tolerance
        void SetSclFluxTol( double scl_flux_tol ) { scl_flux_tol_ = scl_flux_tol; };
        double SclFluxTol() const { return scl_flux_tol_; };

        // Random number seed
        void SetSeed( unsigned int seed ) { seed_ = seed; }; 
        unsigned int Seed() const { return seed_; };

        // Period of progress reports
        void SetProgressPeriod( unsigned int period ) { progress_period_ = period; };
        unsigned int ProgressPeriod() const { return progress_period_; };

        // Friend functions //
 
        // Overload I/O operators
        friend std::ostream &operator<< ( std::ostream &out, const Settings &obj );

    private:

        // Left boundary condition
        BoundaryCondition left_bc_;

        // Fundamental k eigenvalue guess
        double k_guess_;

        // [Adjoint] Fundamental k eigenvalue guess
        double adj_k_guess_;

        // Total fission source guess
        double fission_source_guess_;

        // [Adjoint] Total fission source guess
        double adj_fission_source_guess_;

        // Fundamental k eigenvalue convergence tolerance
        double k_tol_;

        // Scalar flux convergence tolerance
        double scl_flux_tol_;

        // Random number seed
        unsigned int seed_;

        // Period of progress reports
        unsigned int progress_period_;
};

// Friend functions //

// Overload I/O operators
std::ostream &operator<< ( std::ostream &out, const Settings &obj );
