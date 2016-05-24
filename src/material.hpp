// material.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>

// biscotti includes
#include "groupdependent.hpp"
#include "groupgroupdependent.hpp"

class Material
{
    public:

        // Default constructor
        Material();

        // Accessors and mutators //

        void SetMacroAbsXsec( double energy, double value );

        void SetMacroScatXsec( double from_energy, double to_energy, double value );
        const GroupGroupDependent &MacroScatXsec() const { return macro_scat_xsec_; };
        const GroupGroupDependent &AdjMacroScatXsec() const { return adj_macro_scat_xsec_; };

        void SetMacroFissXsec( double energy, double value );
        const GroupDependent &MacroFissXsec() const { return macro_fiss_xsec_; };

        void SetFissNu( double energy, double value );
        const GroupDependent &FissNu() const { return fiss_nu_; };

        void SetFissChi( double energy, double value );
        const GroupDependent &FissChi() const { return fiss_chi_; };

        void SetExtSource( double energy, double value );
        const GroupDependent &ExtSource() const { return ext_source_; };

        void AdjSetExtSource( double energy, double value );
        const GroupDependent &AdjExtSource() const { return adj_ext_source_; };

        const GroupDependent &TotMacroXsec() const { return tot_macro_xsec_; };

        // Friend functions //
        
        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Material &obj );

    private:

        // Macroscopic total cross section
        GroupDependent tot_macro_xsec_;

        // Macroscopic absorption cross section
        GroupDependent macro_abs_xsec_;

        // Macroscopic scattering cross section
        GroupGroupDependent macro_scat_xsec_;

        // [Adjoint] Macroscopic scattering cross section
        GroupGroupDependent adj_macro_scat_xsec_;

        // Macroscopic fission cross section
        GroupDependent macro_fiss_xsec_;

        // Average number of neutrons produced per fission, nu
        GroupDependent fiss_nu_;

        // Distribution of prompt fission neutrons, chi
        GroupDependent fiss_chi_;

        // External source
        GroupDependent ext_source_;

        // [Adjoint] External source
        GroupDependent adj_ext_source_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Material &obj );
