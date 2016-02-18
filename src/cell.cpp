// cell.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>
#include <vector>

// sn-solver includes
#include "cell.hpp"

// Default constructor
Cell::Cell( const Segment &segment ):
    segment_( segment )
{}

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Cell &obj )
{
    out << "Cell address: " << &obj << "\t";
    out << "Segment address: " << &obj.segment_ << "\t";
    return out;
}
