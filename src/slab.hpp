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

        // Friend functions //
 
        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Slab &obj );

    private:

        // Sweep right
        void SweepRight( double energy );

        // Sweep left
        void SweepLeft( double energy );

        // Check if scalar flux is converged
        bool ScalarFluxConverged();

        // Const Settings
        const Settings settings_;

        // Const Layout
        const Layout layout_;

        // Previous k eigenvalue
        double prev_k_;

        // Current k eigenvalue
        double cur_k_;

        // Vector of cells
        std::vector<Cell> cells_;

        // Set of energy groups for problem
        std::set<double> energy_groups_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Slab &obj );
