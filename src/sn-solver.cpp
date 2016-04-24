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
    settings_1.SetLeftBC( Settings::VACUUM );
    settings_1.SetKGuess( 1.0 );
    settings_1.AdjSetKGuess( 1.0 );
    settings_1.SetFissionSourceGuess( 10.0 );
    settings_1.AdjSetFissionSourceGuess( 10.0 );
    settings_1.SetKTol( 1.0e-5 );
    settings_1.SetSclFluxTol( 1.0e-5 );
    settings_1.SetSeed( 10 );
    settings_1.SetProgressPeriod( 1 );

    // Create moderator material reflector //
    Material reflector;

    // Absorption
    reflector.SetMacroAbsXsec( 2.5e-2, 0.05 );
    reflector.SetMacroAbsXsec( 1.0e6, 0.025 );

    // Scattering
    reflector.SetMacroScatXsec( 2.5e-2, 2.5e-2, 0.25 );
    reflector.SetMacroScatXsec( 2.5e-2, 1.0e6, 0.0 );
    reflector.SetMacroScatXsec( 1.0e6, 2.5e-2, 0.1125 );
    reflector.SetMacroScatXsec( 1.0e6, 1.0e6, 0.1125 );

    // Fission
    reflector.SetMacroFissXsec( 2.5e-2, 0.0 );
    reflector.SetMacroFissXsec( 1.0e6, 0.0 );
    reflector.SetFissNu( 2.5e-2, 1.0 );
    reflector.SetFissNu( 1.0e6, 1.0 );
    reflector.SetFissChi( 2.5e-2, 0.0 );
    reflector.SetFissChi( 1.0e6, 1.0 );

    // External source
    reflector.SetExtSource( 2.5e-2, 0.0 );
    reflector.SetExtSource( 1.0e6, 0.0 );
    reflector.AdjSetExtSource( 2.5e-2, 0.0 );
    reflector.AdjSetExtSource( 1.0e6, 0.0 );

    // Create fissile material core //
    Material core;

    // Absorption
    core.SetMacroAbsXsec( 2.5e-2, 1.0 );
    core.SetMacroAbsXsec( 1.0e6, 0.075 );

    // Scattering
    core.SetMacroScatXsec( 2.5e-2, 2.5e-2, 1.0 );
    core.SetMacroScatXsec( 2.5e-2, 1.0e6, 0.0 );
    core.SetMacroScatXsec( 1.0e6, 2.5e-2, 0.001 );
    core.SetMacroScatXsec( 1.0e6, 1.0e6, 0.049 );

    // Fission
    core.SetMacroFissXsec( 2.5e-2, 6.0 );
    core.SetMacroFissXsec( 1.0e6, 0.05 );
    core.SetFissNu( 2.5e-2, 2.5 );
    core.SetFissNu( 1.0e6, 2.8 );
    core.SetFissChi( 2.5e-2, 0.0 );
    core.SetFissChi( 1.0e6, 1.0 );

    // External source
    core.SetExtSource( 2.5e-2, 0.0 );
    core.SetExtSource( 1.0e6, 0.0 );
    core.AdjSetExtSource( 2.5e-2, 0.0 );
    core.AdjSetExtSource( 1.0e6, 0.0 );

    // Create layout //
    Layout layout_1;
    layout_1.AddToEnd( reflector, 25.0, 1000, 1.0, 1.0 );
    layout_1.AddToEnd( core, 30.0, 1000, 1.0, 1.0 );

    // Create slab and run //
    Slab slab_1( settings_1, layout_1 );
    slab_1.EigenvalueSolve();
}
