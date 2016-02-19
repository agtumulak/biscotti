// layout.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <vector>

// sn-solver includes
#include "cell.hpp"
#include "segment.hpp"
#include "settings.hpp"

class Layout
{
    public:

        // Default constructor
        Layout();

        // Add segment to end
        void AddToEnd( Material material, double width, unsigned int num_cells, double scl_flux_guess );

        // Generate cells for use with Slab object
        std::vector<Cell> GenerateCells( const Settings &settings ) const;

        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Layout &obj );

    private:

        // Layout of slab
        std::vector<Segment> data_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Layout &obj );
