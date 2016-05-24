// material.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>

// biscotti includes
#include "material.hpp"

// Default constructor
Material::Material()
{}

// Accessors and mutators //

// Absorption cross section
void Material::SetMacroAbsXsec( double energy, double value )
{
    macro_abs_xsec_.Set( energy, value );
    tot_macro_xsec_.Add( energy, value );
}

// Scattering cross section
void Material::SetMacroScatXsec( double from_energy, double to_energy, double value )
{
    macro_scat_xsec_.Set( from_energy, to_energy, value );
    adj_macro_scat_xsec_.Set( to_energy, from_energy, value );
    tot_macro_xsec_.Add( from_energy, value );
}

// Fission cross section
void Material::SetMacroFissXsec( double energy, double value )
{
    macro_fiss_xsec_.Set( energy, value );
    tot_macro_xsec_.Add( energy, value );
}

// Average number of neutrons per fission, nu
void Material::SetFissNu( double energy, double value )
{
    fiss_nu_.Set( energy, value );
}

// Energy distribution of fission neutrons, chi
void Material::SetFissChi( double energy, double value )
{
    fiss_chi_.Set( energy, value );
}

// External source
void Material::SetExtSource( double energy, double value )
{
    ext_source_.Set( energy, value );
}

// [Adjoint] External source
void Material::AdjSetExtSource( double energy, double value )
{
    adj_ext_source_.Set( energy, value );
}

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Material &obj )
{
    out << std::scientific;

    out << "Macroscopic absorption cross section: \n" << obj.macro_abs_xsec_ << std::endl;
    out << "Macroscopic scattering cross sections: \n" << obj.macro_scat_xsec_ << std::endl;
    out << "Macroscopic fission cross section: \n" << obj.macro_fiss_xsec_ << std::endl;
    out << "Macroscopic total cross section: \n" << obj.tot_macro_xsec_ << std::endl;
    out << "Average number of neutrons per fission, nu: " << obj.fiss_nu_ << "\n" << std::endl;
    out << "Energy distribution of fission neutrons, chi: \n" << obj.fiss_chi_ << std::endl;
    out << "External source: \n" << obj.ext_source_;

    out << std::defaultfloat;

    return out;
}
