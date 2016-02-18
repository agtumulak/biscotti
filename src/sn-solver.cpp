// sn-solver.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>

// sn-solver includes
#include "settings.hpp"
#include "material.hpp"

int main()
{

    // Create settings //
    Settings settings_1;
    settings_1.WriteSeed( 10 );
    settings_1.WriteProgressPeriod( 100 );

    // Create moderator material mat_1 //
    Material mat_1;

    // Absorption
    mat_1.WriteMacroAbsXsec( 0.1, 0.05 );
    mat_1.WriteMacroAbsXsec( 14.0, 0.0 );

    // Scattering
    mat_1.WriteMacroScatXsec( 0.1, 0.1, 0.05 );
    mat_1.WriteMacroScatXsec( 0.1, 14.0, 0.0 );
    mat_1.WriteMacroScatXsec( 14.0, 0.1, 0.1 );
    mat_1.WriteMacroScatXsec( 14.0, 14.0, 0.0 );

    // Fission
    mat_1.WriteMacroFissXsec( 0.1, 0.0 );
    mat_1.WriteMacroFissXsec( 14.0, 0.0 );
    mat_1.WriteFissNu( 1.0 );
    mat_1.WriteFissChi( 0.1, 0.0 );
    mat_1.WriteFissChi( 14.0, 1.0 );

    // External source
    mat_1.WriteExtSource( 0.1, 0.1 );
    mat_1.WriteExtSource( 14.0, 0.0 );

    // Test //

    std::cout << mat_1 << std::endl;
}
