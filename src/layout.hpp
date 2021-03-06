// layout.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <set>
#include <vector>

// biscotti includes
#include "cell.hpp"
#include "segment.hpp"
#include "settings.hpp"

class Layout
{
    public:

        // Default constructor
        Layout();

        // Add segment to end
        void AddToEnd( Material material, double width, unsigned int num_cells, double scl_flux_guess, double adj_scl_flux_guess );

        // Generate cells for use with Slab object
        std::vector<Cell> GenerateCells( const Settings &settings, const double &k, const double &adj_k ) const;

        // Generate energy groups to use in calculation
        std::set<double> GenerateEnergyGroups() const;

        // Generate speed groups to use in calculation (eV to cm/s)
        std::set<double> GenerateSpeedGroups() const;

        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Layout &obj );

    private:

        // Layout of slab
        std::vector<Segment> data_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Layout &obj );
