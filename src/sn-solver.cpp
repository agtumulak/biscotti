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
    settings_1.WriteLeftBC( Settings::VACUUM );
    settings_1.WriteKGuess( 1.0 );
    settings_1.AdjWriteKGuess( 1.0 );
    settings_1.WriteFissionSourceGuess( 10.0 );
    settings_1.AdjWriteFissionSourceGuess( 10.0 );
    settings_1.WriteKTol( 1.0e-5 );
    settings_1.WriteSclFluxTol( 1.0e-5 );
    settings_1.WriteSeed( 10 );
    settings_1.WriteProgressPeriod( 1 );

    // Create moderator material reflector //
    Material reflector;

    // Absorption
    reflector.WriteMacroAbsXsec( 2.5e-2, 0.05 );
    reflector.WriteMacroAbsXsec( 1.0e6, 0.025 );

    // Scattering
    reflector.WriteMacroScatXsec( 2.5e-2, 2.5e-2, 0.25 );
    reflector.WriteMacroScatXsec( 2.5e-2, 1.0e6, 0.0 );
    reflector.WriteMacroScatXsec( 1.0e6, 2.5e-2, 0.1125 );
    reflector.WriteMacroScatXsec( 1.0e6, 1.0e6, 0.1125 );

    // Fission
    reflector.WriteMacroFissXsec( 2.5e-2, 0.0 );
    reflector.WriteMacroFissXsec( 1.0e6, 0.0 );
    reflector.WriteFissNu( 2.5e-2, 1.0 );
    reflector.WriteFissNu( 1.0e6, 1.0 );
    reflector.WriteFissChi( 2.5e-2, 0.0 );
    reflector.WriteFissChi( 1.0e6, 1.0 );

    // External source
    reflector.WriteExtSource( 2.5e-2, 0.0 );
    reflector.WriteExtSource( 1.0e6, 0.0 );
    reflector.AdjWriteExtSource( 2.5e-2, 0.0 );
    reflector.AdjWriteExtSource( 1.0e6, 0.0 );

    // Create fissile material core //
    Material core;

    // Absorption
    core.WriteMacroAbsXsec( 2.5e-2, 1.0 );
    core.WriteMacroAbsXsec( 1.0e6, 0.075 );

    // Scattering
    core.WriteMacroScatXsec( 2.5e-2, 2.5e-2, 1.0 );
    core.WriteMacroScatXsec( 2.5e-2, 1.0e6, 0.0 );
    core.WriteMacroScatXsec( 1.0e6, 2.5e-2, 0.001 );
    core.WriteMacroScatXsec( 1.0e6, 1.0e6, 0.049 );

    // Fission
    core.WriteMacroFissXsec( 2.5e-2, 6.0 );
    core.WriteMacroFissXsec( 1.0e6, 0.05 );
    core.WriteFissNu( 2.5e-2, 2.5 );
    core.WriteFissNu( 1.0e6, 2.8 );
    core.WriteFissChi( 2.5e-2, 0.0 );
    core.WriteFissChi( 1.0e6, 1.0 );

    // External source
    core.WriteExtSource( 2.5e-2, 0.0 );
    core.WriteExtSource( 1.0e6, 0.0 );
    core.AdjWriteExtSource( 2.5e-2, 0.0 );
    core.AdjWriteExtSource( 1.0e6, 0.0 );

    // Create layout //
    Layout layout_1;
    layout_1.AddToEnd( reflector, 25.0, 1000, 1.0, 1.0 );
    layout_1.AddToEnd( core, 30.0, 1000, 1.0, 1.0 );

    // Create slab and run //
    Slab slab_1( settings_1, layout_1 );
    slab_1.EigenvalueSolve();
    slab_1.PrintScalarFluxes();
    slab_1.PrintNeutronDensities();
}
