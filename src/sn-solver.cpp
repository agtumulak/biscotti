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
    settings_1.WriteFissionSourceGuess( 10.0 );
    settings_1.WriteKTol( 1.0e-5 );
    settings_1.WriteSclFluxTol( 1.0e-5 );
    settings_1.WriteSeed( 10 );
    settings_1.WriteProgressPeriod( 1 );
    settings_1.WriteWrongness( 100.0 );

    // Create moderator material reflector //
    Material reflector;

    // Absorption
    reflector.WriteMacroAbsXsec( 0.1, 0.05 );
    reflector.WriteMacroAbsXsec( 14.0, 0.025 );

    // Scattering
    reflector.WriteMacroScatXsec( 0.1, 0.1, 0.25 );
    reflector.WriteMacroScatXsec( 0.1, 14.0, 0.0 );
    reflector.WriteMacroScatXsec( 14.0, 0.1, 0.1125 );
    reflector.WriteMacroScatXsec( 14.0, 14.0, 0.1125 );

    // Fission
    reflector.WriteMacroFissXsec( 0.1, 0.0 );
    reflector.WriteMacroFissXsec( 14.0, 0.0 );
    reflector.WriteFissNu( 0.1, 1.0 );
    reflector.WriteFissNu( 14.0, 1.0 );
    reflector.WriteFissChi( 0.1, 0.0 );
    reflector.WriteFissChi( 14.0, 1.0 );

    // External source
    reflector.WriteExtSource( 0.1, 0.0 );
    reflector.WriteExtSource( 14.0, 0.0 );

    // Create fissile material core //
    Material core;

    // Absorption
    core.WriteMacroAbsXsec( 0.1, 1.0 );
    core.WriteMacroAbsXsec( 14.0, 0.075 );

    // Scattering
    core.WriteMacroScatXsec( 0.1, 0.1, 1.0 );
    core.WriteMacroScatXsec( 0.1, 14.0, 0.0 );
    core.WriteMacroScatXsec( 14.0, 0.1, 0.001 );
    core.WriteMacroScatXsec( 14.0, 14.0, 0.049 );

    // Fission
    core.WriteMacroFissXsec( 0.1, 6.0 );
    core.WriteMacroFissXsec( 14.0, 0.05 );
    core.WriteFissNu( 0.1, 2.5 );
    core.WriteFissNu( 14.0, 2.8 );
    core.WriteFissChi( 0.1, 0.0 );
    core.WriteFissChi( 14.0, 1.0 );

    // External source
    core.WriteExtSource( 0.1, 0.0 );
    core.WriteExtSource( 14.0, 0.0 );

    // Create layout //
    Layout layout_1;
    layout_1.AddToEnd( reflector, 25.0, 1000, 1.0 );
    layout_1.AddToEnd( core, 30.0, 1000, 1.0 );

    // Create slab and run //
    Slab slab_1( settings_1, layout_1 );
    slab_1.EigenvalueSolve();
    slab_1.PrintScalarFluxes();
}
