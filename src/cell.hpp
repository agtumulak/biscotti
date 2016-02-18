// cell.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>

// sn-solver includes
#include "segment.hpp"

class Cell
{
    public:

        // Default constructor
        Cell( const Segment &segment );

        // Accessors and mutators //

        // Return const reference to segment_
        const Segment &SegmentReference() const { return segment_; };

        // Friend functions //

        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Cell &obj );

    private:

        // Const reference to segment
        const Segment &segment_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Cell &obj );
