// settings.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>

class Settings
{
    public:

        // Default constructor
        Settings();

        // Accessors and mutators //

        void WriteSeed( unsigned int seed ) { seed_ = seed; }; 
        unsigned int Seed() const { return seed_; };

        void WriteProgressPeriod( unsigned int period ) { progress_period_ = period; };
        unsigned int ProgressPeriod() const { return progress_period_; };

        // Friend functions //
 
        // Overload I/O operators
        friend std::ostream &operator<< ( std::ostream &out, const Settings &obj );

    private:

        // Random number seed
        unsigned int seed_;

        // Period of progress reports
        unsigned int progress_period_;
};

// Friend functions //

// Overload I/O operators
std::ostream &operator<< ( std::ostream &out, const Settings &obj );
