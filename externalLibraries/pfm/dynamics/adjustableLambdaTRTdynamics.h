/* This file is part of pfm-utils, a set of addons for the Palabos library
 *
 * Copyright (c) 2016- JKU Linz
 * author: Philippe Seil (philippe.seil@jku.at)
 *
 * pfm-utils is free software released under the GPLv3
 */

/*
 * AdjustableLambdaTRTdynamics is a version of TRT dynamics where
 * the "magic parameter" lambda can be set in the constructor. 
 *
 * Values for the magic parameter:
 * 3/16: exact location of bounceback boundaries
 * 1/4: best stability
 * 1/12: removes third-order advection error (best advection)
 * 1/6: removes fourth-order diffusion error (best diffusion)
 *
 * default value is 3/16.
 */

#ifndef ADJUSTABLE_LAMBDA_TRT_DYNAMICS_H
#define ADJUSTABLE_LAMBDA_TRT_DYNAMICS_H

#include "core/globalDefs.h"
#include "basicDynamics/isoThermalDynamics.h"

namespace plb {

#define TRT_BEST_BOUNCEBACK ( 3./16. )
#define TRT_BEST_STABILITY ( 1./4. )
#define TRT_BEST_ADVECTION ( 1./12. )
#define TRT_BEST_DIFFUSION ( 1./6. )

  template<typename T, template<typename U> class Descriptor>
  class AdjustableLambdaTRTdynamics : public IsoThermalBulkDynamics<T,Descriptor> {
  public:
    AdjustableLambdaTRTdynamics(T const omega_, T const lambda_ = TRT_BEST_BOUNCEBACK);
    virtual AdjustableLambdaTRTdynamics<T,Descriptor>* clone() const;

    virtual int getId() const;

    virtual void collide(Cell<T,Descriptor>& cell,
                         BlockStatistics& statistics_);
    
    virtual void collideExternal(Cell<T,Descriptor>& cell, T rhoBar,
                                 Array<T,Descriptor<T>::d> const& j, T thetaBar, BlockStatistics& stat);
    
    virtual T computeEquilibrium(plint iPop, T rhoBar, Array<T,Descriptor<T>::d> const& j,
                                 T jSqr, T thetaBar=T()) const;

  private:
    static int id;
    T const lambda;

    T calcOmegaMin(T const omega)
    {
      return ( 4. - 2.*omega )/( 4.*lambda*omega + 2. - omega );
    }
  };

} /*  namespace plb */

#include "pfm/dynamics/adjustableLambdaTRTdynamics.hh"

#endif /* ADJUSTABLE_LAMBDA_TRT_DYNAMICS_H */
