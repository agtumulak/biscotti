// biscotti.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>

// biscotti includes
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
    settings_1.SetFissionSourceGuess( 1.0 );
    settings_1.AdjSetFissionSourceGuess( 1.0 );
    settings_1.SetKTol( 1.0e-5 );
    settings_1.SetSclFluxTol( 1.0e-5 );
    settings_1.SetSeed( 10 );
    settings_1.SetProgressPeriod( 10 );

    // Define energies (eV) //
    double thermal = 0.025;
    double fast = 1.0e6;

    // Create reflector material
    Material reflector;

    // Absorption
    reflector.SetMacroAbsXsec( fast, 0.025 );
    reflector.SetMacroAbsXsec( thermal, 0.05 );

    // Scattering
    reflector.SetMacroScatXsec( fast, fast, 0.1125 );
    reflector.SetMacroScatXsec( fast, thermal, 0.1125 );
    reflector.SetMacroScatXsec( thermal, fast, 0.0 );
    reflector.SetMacroScatXsec( thermal, thermal, 0.25 );

    // Fission
    reflector.SetMacroFissXsec( fast, 0.0 );
    reflector.SetMacroFissXsec( thermal, 0.0 );
    reflector.SetFissNu( fast, 1.0 );
    reflector.SetFissNu( thermal, 1.0 );
    reflector.SetFissChi( fast, 1.0 );
    reflector.SetFissChi( thermal, 0.0 );

    // External source
    reflector.SetExtSource( fast, 0.0 );
    reflector.SetExtSource( thermal, 0.0 );
    reflector.AdjSetExtSource( fast, 0.0 );
    reflector.AdjSetExtSource( thermal, 0.0 );

    // Create core material //
    Material core;

    // Absorption
    core.SetMacroAbsXsec( fast, 0.075 );
    core.SetMacroAbsXsec( thermal, 1.0 );

    // Scattering
    core.SetMacroScatXsec( fast, fast, 0.049 );
    core.SetMacroScatXsec( fast, thermal, 0.001 );
    core.SetMacroScatXsec( thermal, fast, 0.0 );
    core.SetMacroScatXsec( thermal, thermal, 1.0 );

    // Fission
    core.SetMacroFissXsec( fast, 0.05 );
    core.SetMacroFissXsec( thermal, 6.0 );
    core.SetFissNu( fast, 2.8 );
    core.SetFissNu( thermal, 2.5 );
    core.SetFissChi( fast, 1.0 );
    core.SetFissChi( thermal, 0.0 );

    // External source
    core.SetExtSource( fast, 0.0 );
    core.SetExtSource( thermal, 0.0 );
    core.AdjSetExtSource( fast, 0.0 );
    core.AdjSetExtSource( thermal, 0.0 );

    // Create layout //
    Layout layout_1;
    layout_1.AddToEnd( reflector, 25.0, 250, 1.0, 1.0 );
    layout_1.AddToEnd( core, 30.0, 6000, 1.0, 1.0 );

    // Create slab and run //
    Slab slab_1( settings_1, layout_1 );
    slab_1.EigenvalueSolve();
	slab_1.FissionSourceSolve();
    slab_1.FirstGenerationWeightedSourceSolve();
    slab_1.FissionMatrixSolve();
}
