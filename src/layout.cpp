// layout.cpp
// Aaron G. Tumulak

// std includes
#include <cassert>
#include <iostream>
#include <vector>

// sn-solver includes
#include "cell.hpp"
#include "layout.hpp"
#include "segment.hpp"

// Default constructor
Layout::Layout()
{}

// Add segment to end
void Layout::AddToEnd( Material material, double width, unsigned int num_cells )
{
    assert( num_cells > 0 );
    data_.push_back( Segment( material, width, num_cells ) );
}

// Generate cells for use with Slab object
std::vector<Cell> Layout::GenerateCells() const
{
    assert( !data_.empty() );
    std::vector<Cell> output;
    // Iterate through each segment in layout
    for( auto segment_it = data_.begin(); segment_it != data_.end(); segment_it++ )
    {
        for( int i = 0; i != segment_it->NumCells(); i++ )
        {
            output.push_back( Cell( *segment_it ) );
        }
    }
    return output;
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
