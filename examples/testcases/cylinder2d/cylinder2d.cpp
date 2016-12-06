/* This file is part of the Palabos library.
 *
 * Copyright (C) 2011-2015 FlowKit Sarl
 * Route d'Oron 2
 * 1010 Lausanne, Switzerland
 * E-mail contact: contact@flowkit.com
 *
 * The most recent release of Palabos can be downloaded at 
 * <http://www.palabos.org/>
 *
 * The library Palabos is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * The library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \file
 * Flow around a 2D cylinder inside a channel, with the creation of a von
 * Karman vortex street. This example makes use of bounce-back nodes to
 * describe the shape of the cylinder. The outlet is modeled through a
 * Neumann (zero velocity-gradient) condition.
 */

#include "palabos3D.h"
#include "palabos3D.hh"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "pfm/utils/PlbCaseTimer/plbCaseTimer.h"
#include "pfm/utils/ProbeManager/probeManager.h"

using namespace plb;
using namespace plb::descriptors;
using namespace std;

typedef double T;
#define DESCRIPTOR MRTD3Q19Descriptor

/// Velocity on the parabolic Poiseuille profile
T poiseuilleVelocity(plint iY, IncomprFlowParam<T> const& parameters) {
  T y = (T)iY / parameters.getResolution();
  return 4.*parameters.getLatticeU() * (y-y*y);
}

/// Linearly decreasing pressure profile
T poiseuillePressure(plint iX, IncomprFlowParam<T> const& parameters) {
  T Lx = parameters.getNx()-1;
  T Ly = parameters.getNy()-1;
  return 8.*parameters.getLatticeNu()*parameters.getLatticeU() / (Ly*Ly) * (Lx/(T)2-(T)iX);
}

/// Convert pressure to density according to ideal gas law
T poiseuilleDensity(plint iX, IncomprFlowParam<T> const& parameters) {
  return poiseuillePressure(iX,parameters)*DESCRIPTOR<T>::invCs2 + (T)1;
}

/// A functional, used to initialize the velocity for the boundary conditions
template<typename T>
class PoiseuilleVelocity {
public:
  PoiseuilleVelocity(IncomprFlowParam<T> parameters_)
    : parameters(parameters_)
  { }
  void operator()(plint iX, plint iY, plint iZ, Array<T,3>& u) const {
    u[0] = poiseuilleVelocity(iY, parameters);
    u[1] = T();
    u[2] = T();
  }
private:
  IncomprFlowParam<T> parameters;
};

/// A functional, used to initialize a pressure boundary to constant density
template<typename T>
class ConstantDensity {
public:
  ConstantDensity(T density_)
    : density(density_)
  { }
  T operator()(plint iX, plint iY, plint iZ) const {
    return density;
  }
private:
  T density;
};

/// A functional, used to create an initial condition for the density and velocity
template<typename T>
class PoiseuilleVelocityAndDensity {
public:
  PoiseuilleVelocityAndDensity(IncomprFlowParam<T> parameters_)
    : parameters(parameters_)
  { }
  void operator()(plint iX, plint iY, plint iZ, T& rho, Array<T,3>& u) const {
    rho = poiseuilleDensity(iX,parameters);
    u[0] = poiseuilleVelocity(iY, parameters);
    u[1] = T();
    u[2] = T();
  }
private:
  IncomprFlowParam<T> parameters;
};

/// A functional, used to instantiate bounce-back nodes at the locations of the cylinder
template<typename T>
class CylinderShapeDomain3D : public plb::DomainFunctional3D {
public:
  CylinderShapeDomain3D(plb::plint cx_, plb::plint cy_, plb::plint radius)
    : cx(cx_),
      cy(cy_),
      radiusSqr(plb::util::sqr(radius))
  { }
  virtual bool operator() (plb::plint iX, plb::plint iY, plb::plint iZ) const {
    return plb::util::sqr(iX-cx) + plb::util::sqr(iY-cy) <= radiusSqr;
  }
  virtual CylinderShapeDomain3D<T>* clone() const {
    return new CylinderShapeDomain3D<T>(*this);
  }
private:
  plb::plint cx;
  plb::plint cy;
  plb::plint radiusSqr;
};


void cylinderSetup( MultiBlockLattice3D<T,DESCRIPTOR>& lattice,
                    IncomprFlowParam<T> const& parameters,
                    OnLatticeBoundaryCondition3D<T,DESCRIPTOR>& boundaryCondition,
                    plint const cx, plint const cy, plint const radius)
{
  const plint nx = lattice.getNx();
  const plint ny = lattice.getNy();
  const plint nz = lattice.getNz();

  lattice.periodicity().toggle(0,false);
  lattice.periodicity().toggle(1,false);
  lattice.periodicity().toggle(2,true);


  Box3D inlet(0,0, 1, ny-2,0,nz-1);
  Box3D outlet(nx-1,nx-1, 1, ny-2,0,nz-1);
  Box3D leftWall(0, nx-1, 0, 0, 0, nz-1), rightWall(0, nx-1, ny-1, ny-1, 0, nz-1);
  // Create Velocity boundary conditions everywhere
  boundaryCondition.setVelocityConditionOnBlockBoundaries (lattice, inlet );
  boundaryCondition.setVelocityConditionOnBlockBoundaries (lattice, leftWall );
  boundaryCondition.setVelocityConditionOnBlockBoundaries (lattice, rightWall );

  // boundaryCondition.setPressureConditionOnBlockBoundaries (lattice, outlet, boundary::outflow );
  //  boundaryCondition.setPressureConditionOnBlockBoundaries (lattice, outlet);
  boundaryCondition.setVelocityConditionOnBlockBoundaries (lattice, outlet, boundary::outflow);
  // boundaryCondition.setVelocityConditionOnBlockBoundaries (lattice, outlet );

  setBoundaryVelocity (lattice, inlet, PoiseuilleVelocity<T>(parameters) );
  setBoundaryVelocity (lattice, outlet, PoiseuilleVelocity<T>(parameters) );
  setBoundaryVelocity (lattice, leftWall, Array<T,3>(0.,0.,0.) );
  setBoundaryVelocity (lattice, rightWall, Array<T,3>(0.,0.,0.) );

  // setBoundaryDensity(lattice,outlet,1.);

  initializeAtEquilibrium (lattice, lattice.getBoundingBox(),PoiseuilleVelocityAndDensity<T>(parameters) );

  defineDynamics(lattice, lattice.getBoundingBox(),
                 new CylinderShapeDomain3D<T>(cx,cy,radius),
                 new plb::BounceBack<T,DESCRIPTOR>);

  lattice.initialize();
}

void writeGif(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, plint iter)
{
  ImageWriter<T> imageWriter("leeloo");
  Box3D writeBox(0,lattice.getNx()-1,
                 0,lattice.getNy()-1,
                 lattice.getNz()/2,lattice.getNz()/2);
  imageWriter.writeGif(createFileName("u", iter, 6),
                       *computeVelocityNorm(lattice,writeBox), 0, 0.01, 600, 600 );
}

void writeVTK(MultiBlockLattice3D<T,DESCRIPTOR>& lattice,
              IncomprFlowParam<T> const& parameters, plint iter)
{
  T dx = parameters.getDeltaX();
  T dt = parameters.getDeltaT();
  VtkImageOutput3D<T> vtkOut(createFileName("vtk", iter, 6), dx);
  vtkOut.writeData<float>(*computeVelocityNorm(lattice), "velocityNorm", dx/dt);
  vtkOut.writeData<3,float>(*computeVelocity(lattice), "velocity", dx/dt);
}


int main(int argc, char* argv[]) {
  plbInit(&argc, &argv);

  PlbCaseTimer timer;
  timer.registerCustom("CollideAndStream");
  timer.registerCustom("Output");
  timer.registerCustom("Setup");
  timer.registerCustom("Logging");
  timer.startGlobal();
  timer.startCustom("Setup");

  global::directories().setOutputDir("./tmp/");

  IncomprFlowParam<T> parameters((T) 1e-2,  // uMax
                                 (T) 600.,  // Re
                                 80,        // N
                                 6.,        // lx
                                 1.         // ly 
                                 );
  const T logT     = (T)0.02;
  const T imSave   = (T)0.06;
  const T vtkSave  = (T)1.;
  const T probeT   = (T)0.01;
  const T maxT     = (T)0.10;

  writeLogFile(parameters, "Poiseuille flow");

//  MultiBlockLattice3D<T, DESCRIPTOR> lattice (parameters.getNx(), parameters.getNy(), 4,
//                                             new MRTdynamics<T,DESCRIPTOR>(parameters.getOmega()) );

  MultiBlockLattice3D<T, DESCRIPTOR> lattice (parameters.getNx(), parameters.getNy(), 4,
                                              new BGKdynamics<T,DESCRIPTOR>(parameters.getOmega()) );

  OnLatticeBoundaryCondition3D<T,DESCRIPTOR>*
    boundaryCondition = createLocalBoundaryCondition3D<T,DESCRIPTOR>();

  T const cx = 1.5, cy = 0.52, rc = 0.15;

  cylinderSetup(lattice, parameters, *boundaryCondition, 
                parameters.nCell(cx), parameters.nCell(cy), parameters.nCell(rc));

  std::vector<Array<T,3> > velProbeLocations, rhoProbeLocations;
  velProbeLocations.push_back(Array<T,3>(parameters.nCell(cx+2*rc),parameters.nCell(cy),1));
  velProbeLocations.push_back(Array<T,3>(parameters.nCell(cx+4*rc),parameters.nCell(cy),1));

  rhoProbeLocations.push_back(Array<T,3>(parameters.nCell(cx+2*rc),parameters.nCell(cy),1));
  rhoProbeLocations.push_back(Array<T,3>(parameters.nCell(cx+4*rc),parameters.nCell(cy),1));

  std::string velFname("tmp/velprobes.txt");
  std::string rhoFname("tmp/rhoprobes.txt");

  ProbeManager<T,DESCRIPTOR> pM(velFname,rhoFname,velProbeLocations,rhoProbeLocations,parameters);
  pM.writeHeaders();

  timer.stopCustom("Setup");

  // Main loop over time iterations.
  for (pluint iT=0; iT*parameters.getDeltaT()<maxT; ++iT) {
    // At this point, the state of the lattice corresponds to the
    //   discrete time iT. However, the stored averages (getStoredAverageEnergy

    //   and getStoredAverageDensity) correspond to the previous time iT-1.

    // if (iT%parameters.nStep(imSave)==0) {
    //   pcout << "Saving Gif ..." << endl;
    //   writeGif(lattice, iT);
    // }

    if (iT%parameters.nStep(probeT)==0) {
      pcout << "Saving probes ..." << endl;
      timer.startCustom("Output");
      pM.writeVelocityProbes(lattice,iT);
      pM.writeDensityProbes(lattice,iT);
      timer.stopCustom("Output");
    }
    
    // if (iT%parameters.nStep(vtkSave)==0 && iT>0) {
    //   pcout << "Saving VTK file ..." << endl;
    //   writeVTK(lattice, parameters, iT);
    // }
    
    if (iT%parameters.nStep(logT)==0) {
      pcout << "step " << iT
            << "; t=" << iT*parameters.getDeltaT();
    }

    // Lattice Boltzmann iteration step.
    timer.startCustom("CollideAndStream");
    lattice.collideAndStream();
    timer.stopCustom("CollideAndStream");

    // At this point, the state of the lattice corresponds to the
    //   discrete time iT+1, and the stored averages are upgraded to time iT.
    if (iT%parameters.nStep(logT)==0) {
      timer.startCustom("Logging");
      pcout << "; av energy ="
            << setprecision(10) << getStoredAverageEnergy<T>(lattice)
            << "; av rho ="
            << getStoredAverageDensity<T>(lattice) << endl;
      timer.stopCustom("Logging");
    }
  }
    
  delete boundaryCondition;

  timer.stopGlobal();
  timer.writeLog(pcout);
  std::string fname("timer.json");
  timer.writeLogJSON(fname);
}
