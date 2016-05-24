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
    // Problem settings //

    // Create a settings object called settings_1. This object will be passed
    // into a Slab object for use.
    Settings settings_1;

    // Set the settings by using the Setting object's member functions
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

    // Create the materials that will be used in the problem. There is no need
    // to ever create two materials that have the same exact properties.

    // Create a Material object called reflector
    Material reflector;

    // Set the properties of the reflector object.

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

    // If solving a fixed-source problem, specify an external source.
    // Otherwise, leave these values at 0.0

    // External source
    reflector.SetExtSource( fast, 0.0 );
    reflector.SetExtSource( thermal, 0.0 );
    reflector.AdjSetExtSource( fast, 0.0 );
    reflector.AdjSetExtSource( thermal, 0.0 );

    // Create another material for the core. The construction of each new
    // material is similar. You can create as many unique materials as needed.

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

    // With the materials defined, you now define a 1-D slab. Each slab is made
    // up of sections. Each section is assigned a material, a width, a number
    // of cells, an initial guess for the scalar flux, and an initial guess for
    // the adjoint scalar flux. The function AddToEnd() stacks new sections to
    // the end of the slab. You can think of this as literally stacking
    // sections of a slab reactor.

    // Create layout //
    Layout layout_1;
    layout_1.AddToEnd( reflector, 25.0, 250, 1.0, 1.0 );
    layout_1.AddToEnd( core, 30.0, 6000, 1.0, 1.0 );

    // With the Layout object created, initialize a Slab object using the
    // Settings object and the Layout object you have created.
    Slab slab_1( settings_1, layout_1 );

    // The Slab object drives the calculations you want to perform. To perform
    // a k-eigenvalue solve, you run the EigenvalueSolve() method on the slab.
    slab_1.EigenvalueSolve();

	// slab_1.FissionSourceSolve();
    // slab_1.FirstGenerationWeightedSourceSolve();
    // slab_1.FissionMatrixSolve();
}
