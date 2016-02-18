// segment.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>

// sn-solver includes
#include "material.hpp"

class Segment
{
    public:

        // Default constructor
        Segment( Material material, double width, int num_cells );

        // Accessors and mutators //

        // Return const reference to material_
        const Material &MaterialReference() const { return material_; };

        // Read width
        double Width() const { return width_; };

        // Read number of equally spaced cells
        int NumCells() const { return num_cells_; };

        // Read cell width
        double CellWidth() const { return cell_width_; };

        // Friend functions //

        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Segment &obj );

    private:

        // Material
        const Material material_;

        // Width
        const double width_;

        // Number of equally spaced cells
        const int num_cells_;

        // Cell width
        const double cell_width_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Segment &obj );
