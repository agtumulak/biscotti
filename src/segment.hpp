// segment.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <map>

// biscotti includes
#include "angledependent.hpp"
#include "material.hpp"
#include "settings.hpp"

class Segment
{
    public:

        // Default constructor
        Segment( Material material, double width, unsigned int num_cells, double scl_flux_guess, double adj_scl_flux_guess );

        // Accessors and mutators //

        // Return const reference to material_
        const Material &MaterialReference() const { return material_; };

        // Read width
        double Width() const { return width_; };

        // Read number of equally spaced cells
        int NumCells() const { return num_cells_; };

        // Read cell width
        double CellWidth() const { return cell_width_; };

        // Read scalar flux guess
        double ScalarFluxGuess() const { return scl_flux_guess_; };

        // [Adjoint] Read scalar flux guess
        double AdjScalarFluxGuess() const { return adj_scl_flux_guess_; };

        // Friend functions //

        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Segment &obj );

    private:

        // Material
        const Material material_;

        // Width
        const double width_;

        // Number of equally spaced cells
        const unsigned int num_cells_;

        // Cell width
        const double cell_width_;

        // Scalar flux guess
        double scl_flux_guess_;

        // [Adjoint] Scalar flux guess
        double adj_scl_flux_guess_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Segment &obj );
