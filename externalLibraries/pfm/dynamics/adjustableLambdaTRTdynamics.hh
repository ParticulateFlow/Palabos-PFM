/* This file is part of pfm-utils, a set of addons for the Palabos library
 *
 * Copyright (c) 2016- JKU Linz
 * author: Philippe Seil (philippe.seil@jku.at)
 *
 * pfm-utils is free software released under the GPLv3
 */

#ifndef ADJUSTABLE_LAMBDA_TRT_DYNAMICS_HH
#define ADJUSTABLE_LAMBDA_TRT_DYNAMICS_HH

#include "pfm/dynamics/adjustableLambdaTRTdynamics.h"

namespace plb{

  template<typename T, template<typename U> class Descriptor>
  int AdjustableLambdaTRTdynamics<T,Descriptor>::id =
    meta::registerOneParamDynamics<T,Descriptor,TRTdynamics<T,Descriptor> >("AdjustableLambdaTRT");

  template<typename T, template<typename U> class Descriptor>
  AdjustableLambdaTRTdynamics<T,Descriptor>::AdjustableLambdaTRTdynamics(T const omega_, T const lambda_)
    : IsoThermalBulkDynamics<T,Descriptor>(omega_),
    lambda(lambda_)
  { }

  template<typename T, template<typename U> class Descriptor>
  AdjustableLambdaTRTdynamics<T,Descriptor>* AdjustableLambdaTRTdynamics<T,Descriptor>::clone() const
  {
    return new AdjustableLambdaTRTdynamics<T,Descriptor>(*this);
  }
 
  template<typename T, template<typename U> class Descriptor>
  int AdjustableLambdaTRTdynamics<T,Descriptor>::getId() const
  {
    return id;
  }

  template<typename T, template<typename U> class Descriptor>
  void AdjustableLambdaTRTdynamics<T,Descriptor>::collide (Cell<T,Descriptor>& cell, BlockStatistics& statistics )
  {
    Array<T,3> j;
    T rhoBar;
    momentTemplates<T,Descriptor>::get_rhoBar_j(cell, rhoBar, j);
    T const dummyThetaBar(0);
    collideExternal(cell,rhoBar,j,dummyThetaBar,statistics);
  }

  template<typename T, template<typename U> class Descriptor>
  void AdjustableLambdaTRTdynamics<T,Descriptor>::collideExternal (Cell<T,Descriptor>& cell, T rhoBar,
                                                   Array<T,Descriptor<T>::d> const& j,
                                                   T thetaBar, BlockStatistics& statistics )
  {
    T jSqr = normSqr(j);
    T invRho = Descriptor<T>::invRho(rhoBar);

    Array<T,Descriptor<T>::q> eq;
    dynamicsTemplates<T,Descriptor>::bgk_ma2_equilibria(rhoBar, invRho, j, jSqr, eq);

    Array<T,Descriptor<T>::q/2+1> eq_plus, eq_minus, f_plus, f_minus;

    for (plint i=1; i<=Descriptor<T>::q/2; ++i) {
        eq_plus[i]  = 0.5*(eq[i] + eq[i+Descriptor<T>::q/2]);
        eq_minus[i] = 0.5*(eq[i] - eq[i+Descriptor<T>::q/2]);
        f_plus[i]   = 0.5*(cell[i] + cell[i+Descriptor<T>::q/2]);
        f_minus[i]  = 0.5*(cell[i] - cell[i+Descriptor<T>::q/2]);
    }

    T const omega_plus = this->getOmega();
    T const omega_min = calcOmegaMin(omega_plus);

    cell[0] += -omega_plus*cell[0] + omega_min*eq[0];

    for (plint i=1; i<=Descriptor<T>::q/2; ++i) {
        cell[i] += -omega_plus*(f_plus[i]-eq_plus[i]) - omega_min*(f_minus[i]-eq_minus[i]);
        cell[i+Descriptor<T>::q/2] += -omega_plus*(f_plus[i]-eq_plus[i]) + omega_min*(f_minus[i]-eq_minus[i]);
    }
    
    if (cell.takesStatistics()) {
        gatherStatistics(statistics, rhoBar, jSqr * invRho * invRho );
    }
  }


  template<typename T, template<typename U> class Descriptor>
  T AdjustableLambdaTRTdynamics<T,Descriptor>::computeEquilibrium(plint iPop, T rhoBar,
                                                  Array<T,Descriptor<T>::d> const& j,
                                                  T jSqr, T thetaBar) const
  {
    T invRho = Descriptor<T>::invRho(rhoBar);
    return dynamicsTemplates<T,Descriptor>::bgk_ma2_equilibrium(iPop, rhoBar, invRho, j, jSqr);
  }

  
}; /* namespace plb */

#endif /* ADJUSTABLE_LAMBDA_TRT_DYNAMICS_HH */
