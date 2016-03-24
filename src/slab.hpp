// slab.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <vector>
#include <set>

// sn-solver includes
#include "cell.hpp"
#include "layout.hpp"
#include "settings.hpp"

class Slab
{
    public:

        // Default constructor
        Slab( const Settings &settings, const Layout &layout );

        // Solve for k eigenvalue
        void EigenvalueSolve();

        // [Adjoint] Solve for k eigenvalue
        void AdjEigenvalueSolve();

        // Solve for fixed source
        void FixedSourceSolve();

        // Print scalar fluxes
        void PrintScalarFluxes();

        // Print angular fluxes
        void PrintAngularFluxes();

        // Friend functions //
 
        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Slab &obj );

    private:

        // Sweep right
        void SweepRight();

        // [Adjoint] Sweep right
        void AdjSweepRight();

        // Sweep left
        void SweepLeft();

        // [Adjoint] Sweep left
        void AdjSweepLeft();

        // Check if k eigenvalue is converged. If not, create new fission source.
        bool KConverged();

        // [Adjoint] Check if k eigenvalue is converged. If not, create new fission source.
        bool AdjKConverged();

        // Check if scalar flux is converged
        bool ScalarFluxConverged( unsigned int i );

        // Calculate new cell scatter sources
        void UpdateScatterSources();

        // [Adjoint] Calculate new cell scatter sources
        void AdjUpdateScatterSources();

        // Calculate new cell fission sources
        void UpdateFissionSources();

        // [Adjoint] Calculate new cell fission sources
        void AdjUpdateFissionSources();

        // Const Settings
        const Settings settings_;

        // Const Layout
        const Layout layout_;

        // Current k eigenvalue
        double cur_k_;

        // Previous k eigenvalue
        double prev_k_;

        // Current fission source
        double cur_fission_source_;

        // Previous fission source
        double prev_fission_source_;

        // Vector of cells
        std::vector<Cell> cells_;

        // Set of energy groups for problem
        std::set<double> energy_groups_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Slab &obj );
