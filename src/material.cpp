// material.cpp
// Aaron G. Tumulak

// std includes
#include <iostream>

// sn-solver includes
#include "material.hpp"

// Default constructor
Material::Material()
{}

// Accessors and mutators //

// Absorption cross section
void Material::WriteMacroAbsXsec( double energy, double value )
{
    macro_abs_xsec_.Write( energy, value );
    tot_macro_xsec_.Add( energy, value );
}

// Scattering cross section
void Material::WriteMacroScatXsec( double from_energy, double to_energy, double value )
{
    macro_scat_xsec_.Write( from_energy, to_energy, value );
    tot_macro_xsec_.Add( from_energy, value );
}

// Fission cross section
void Material::WriteMacroFissXsec( double energy, double value )
{
    macro_fiss_xsec_.Write( energy, value );
    tot_macro_xsec_.Add( energy, value );
}

// Average number of neutrons per fission, nu
void Material::WriteFissNu( double energy, double value )
{
    fiss_nu_.Write( energy, value );
}

// Energy distribution of fission neutrons, chi
void Material::WriteFissChi( double energy, double value )
{
    fiss_chi_.Write( energy, value );
}

// External source
void Material::WriteExtSource( double energy, double value )
{
    ext_source_.Write( energy, value );
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
