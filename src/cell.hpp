// cell.hpp
// Aaron G. Tumulak

#pragma once

// std includes
#include <iostream>
#include <map>

// sn-solver includes
#include "angularflux.hpp"
#include "segment.hpp"
#include "settings.hpp"

class Cell
{
    public:

        // Default constructor
        Cell( const Settings &settings, const Segment &segment, const double &k, const double &adj_k );

        // Sweep right
        void SweepRight( const AngularFlux &in_angflux );

        // [Adjoint] Sweep right
        void AdjSweepRight( const AngularFlux &in_angflux );

        // Sweep left
        void SweepLeft( const AngularFlux &in_angflux );

        // [Adjoint] Sweep left
        void AdjSweepLeft( const AngularFlux &in_angflux );

        // Vacuum boundary (incoming on left side)
        void LeftVacuumBoundary();

        // [Adjoint] Vacuum boundary (outgoing on left side)
        void AdjLeftVacuumBoundary();

        // Reflect boundary (reflecting on left side, negative->positive)
        void LeftReflectBoundary();

        // [Adjoint] Reflect boundary (reflecting on left side, positive->negative)
        void AdjLeftReflectBoundary();

        // Reflect boundary (reflecting on right side, positive->negative)
        void RightReflectBoundary();

        // [Adjoint] Reflect boundary (reflecting on right side, negative->positive)
        void AdjRightReflectBoundary();

        // Return scalar flux error
        double MaxAbsScalarFluxError();

        // [Adjoint] Return scalar flux error
        double AdjMaxAbsScalarFluxError();

        // Update midpoint scattering source term
        void UpdateMidpointScatteringSource();

        // [Adjoint] Update midpoint scattering source term
        void AdjUpdateMidpointScatteringSource();

        // Update midpoint fission source term
        void UpdateMidpointFissionSource();

        // [Adjoint] Update midpoint fission source term
        void AdjUpdateMidpointFissionSource();

        // Accessors and mutators //

        // Return cell width
        double Width() const { return segment_.CellWidth(); };

        // Return cell fission source
        double FissionSource() const { return mid_fiss_src_.GroupSum() * segment_.CellWidth(); };

        // [Adjoint] Return cell fission source
        double AdjFissionSource() const { return adj_mid_fiss_src_.GroupSum() * segment_.CellWidth(); };

        // Set external source to given value
        void SetExternalSource( const GroupDependent &value ) { mid_ext_src_ = value; };

        // [Adjoint] Set external source to given value
        void AdjSetExternalSource( const GroupDependent &value ) { adj_mid_ext_src_ = value; };

        // Const reference to outgoing angular flux
        const AngularFlux &OutgoingAngularFluxReference() const { return out_angflux_; };

        // [Adjoint] Const reference to outgoing angular flux
        const AngularFlux &AdjOutgoingAngularFluxReference() const { return adj_out_angflux_; };

        // Reference to midpoint angular flux
        AngularFlux &MidpointAngularFluxReference() { return mid_angflux_; };

        // [Adjoint] Reference to midpoint angular flux
        AngularFlux &AdjMidpointAngularFluxReference() { return adj_mid_angflux_; };

        // Const reference to material
        const Material &MaterialReference() const { return material_; };

        // Friend functions //

        // Overload operator<<()
        friend std::ostream &operator<< ( std::ostream &out, const Cell &obj );

    private:

        // Const reference to settings
        const Settings &settings_;

        // Const reference to segment
        const Segment &segment_;

        // Const reference to material
        const Material &material_;

        // Const reference to k eigenvalue
        const double &k_;

        // [Adjoint] Const reference to k eigenvalue
        const double &adj_k_;

        // External source term
        GroupDependent mid_ext_src_;

        // [Adjoint] External source term
        GroupDependent adj_mid_ext_src_;

        // Midpoint group angular flux
        AngularFlux mid_angflux_;

        // [Adjoint] Midpoint group angular flux
        AngularFlux adj_mid_angflux_;

        // Outgoing boundary group angular flux
        AngularFlux out_angflux_;

        // [Adjoint] Outgoing boundary group angular flux
        AngularFlux adj_out_angflux_;

        // Previous midpoint scalar flux
        GroupDependent prev_mid_sclflux_;

        // [Adjoint] Previous midpoint scalar flux
        GroupDependent adj_prev_mid_sclflux_;

        // Midpoint scattering source term
        GroupDependent mid_scat_src_;

        // [Adjoint] Midpoint scattering source term
        GroupDependent adj_mid_scat_src_;

        // Midpoint fisson source term
        GroupDependent mid_fiss_src_;

        // [Adjoint] Midpoint fisson source term
        GroupDependent adj_mid_fiss_src_;
};

// Friend functions //

// Overload operator<<()
std::ostream &operator<< ( std::ostream &out, const Cell &obj );
