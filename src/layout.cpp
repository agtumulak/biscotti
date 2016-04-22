// layout.cpp
// Aaron G. Tumulak

// std includes
#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

// sn-solver includes
#include "cell.hpp"
#include "layout.hpp"
#include "segment.hpp"

// Default constructor
Layout::Layout()
{}

// Add segment to end
void Layout::AddToEnd( Material material, double width, unsigned int num_cells, double scl_flux_guess, double adj_scl_flux_guess )
{
    assert( num_cells > 0 );
    data_.push_back( Segment( material, width, num_cells, scl_flux_guess, adj_scl_flux_guess ) );
}

// Generate cells for use with Slab object
std::vector<Cell> Layout::GenerateCells( const Settings &settings, const double &k, const double &adj_k ) const
{
    assert( !data_.empty() );
    std::vector<Cell> output;
    // Iterate through each segment in layout
    for( auto segment_it = data_.begin(); segment_it != data_.end(); segment_it++ )
    {
        for( int i = 0; i != segment_it->NumCells(); i++ )
        {
            output.push_back( Cell( settings, *segment_it, k, adj_k ) );
        }
    }
    return output;
}

// Generate energy groups to use in calculation
std::set<double> Layout::GenerateEnergyGroups() const
{
    std::set<double> energy_groups;
    // Iterate through each segment in layout
    for( auto segment_it = data_.begin(); segment_it != data_.end(); segment_it++ )
    {
        // Iterate through each total cross section in segment
        for( auto energy_it = segment_it->MaterialReference().TotMacroXsec().slowest();
                energy_it != std::next( segment_it->MaterialReference().TotMacroXsec().fastest() );
                energy_it++ )
        {
            energy_groups.insert( energy_it->first );
        }
    }
    return energy_groups;
}

// Generate speed groups to use in calculation (eV to cm/s)
std::set<double> Layout::GenerateSpeedGroups() const
{
    // Speed of light (cm/s)
    double c = 2.998e10;
    // Neutron mass (eV/c^2)
    double m = 939.5654e6;

    std::set<double> speed_groups;
    std::set<double> energy_groups = GenerateEnergyGroups();
    for( auto it = energy_groups.begin(); it != energy_groups.end(); it++ )
    {
        speed_groups.insert( c * sqrt( 2.0 * *it /m ) );
    }
    return speed_groups;
}

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Layout &obj )
{
    for( auto it = obj.data_.begin(); it != obj.data_.end(); it++ )
    {
        out << "Segment index: " << it - obj.data_.begin() << std::endl;
        out << *it << std::endl;
    }
    return out;
}
