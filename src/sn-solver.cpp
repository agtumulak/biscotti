// sn-solver.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>

// sn-solver includes
#include "material.hpp"
#include "layout.hpp"
#include "settings.hpp"
#include "slab.hpp"

int main()
{
    // Create settings //
    Settings settings_1;
    settings_1.WriteKGuess( 1.0 );
    settings_1.WriteKTol( 1.0e-5 );
    settings_1.WriteSclFluxTol( 1.0e-5 );
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

    // Create fissile material mat_2 //
    Material mat_2;

    // Absorption
    mat_2.WriteMacroAbsXsec( 0.1, 0.0 );
    mat_2.WriteMacroAbsXsec( 14.0, 0.1 );

    // Scattering
    mat_2.WriteMacroScatXsec( 0.1, 0.1, 0.05 );
    mat_2.WriteMacroScatXsec( 0.1, 14.0, 0.0 );
    mat_2.WriteMacroScatXsec( 14.0, 0.1, 0.0 );
    mat_2.WriteMacroScatXsec( 14.0, 14.0, 0.0 );

    // Fission
    mat_2.WriteMacroFissXsec( 0.1, 0.05 );
    mat_2.WriteMacroFissXsec( 14.0, 0.0 );
    mat_2.WriteFissNu( 1.0 );
    mat_2.WriteFissChi( 0.1, 0.0 );
    mat_2.WriteFissChi( 14.0, 1.0 );

    // External source
    mat_2.WriteExtSource( 0.1, 0.1 );
    mat_2.WriteExtSource( 14.0, 0.0 );

    // Create layout //
    Layout layout_1;
    layout_1.AddToEnd( mat_1, 50.0, 500, 1.0 );
    layout_1.AddToEnd( mat_2, 50.0, 500, 1.0 );

    // Create slab and run //
    Slab slab_1( settings_1, layout_1 );
    slab_1.Solve();
}
