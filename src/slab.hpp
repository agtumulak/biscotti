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

        // Solve
        void Solve();

        // Print scalar fluxes
        void PrintScalarFluxes() const;

        // Friend functions //
 
        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Slab &obj );

    private:

        // Sweep right
        void SweepRight();

        // Sweep left
        void SweepLeft();

        // Check if scalar flux is converged
        bool ScalarFluxConverged();

        // Calculate new source term for each cell and update K
        void UpdateSourceAndK();

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
