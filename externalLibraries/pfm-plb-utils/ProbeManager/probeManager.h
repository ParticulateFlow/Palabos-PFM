/*
  Probe Manager
  
  A simple class to keep track of velocity and density probes
  and write their data to files in a consistent way

  ProbeManager is free software released under the GPLv3

  Author: Philippe Seil (philippe.seil@jku.at)
*/

#ifndef PROBEMANAGER_H
#define PROBEMANAGER_H

#include "palabos3D.h"
#include "palabos3D.hh"

#include <fstream>

namespace plb{

  template<typename T, template<typename U> class Descriptor>
  class ProbeManager {
  public:
    typedef std::vector<Array<T,3> > ArrayVector ;
    typedef std::vector<T> ScalarVector;
    ProbeManager(std::string const &velocityFname_, 
                 std::string const &densityFname_, 
                 ArrayVector const &velocityProbeLocations_,
                 ArrayVector const &densityProbeLocations_,
                 IncomprFlowParam<T> const &param_);

    void writeHeaders();

    void writeVelocityProbes(MultiBlockLattice3D<T,Descriptor> &lattice, plint const iT);
    void writeDensityProbes(MultiBlockLattice3D<T,Descriptor> &lattice, plint const iT);
  private:
    std::string velFname,rhoFname;
    ArrayVector velProbes, rhoProbes;
    IncomprFlowParam<T> parameters;

    void writeProbeCoords(plb_ofstream &s, ArrayVector &probes);

    static plint const linewidth = 14;
    static plint const precision = 6;
  };

}; // namespace plb

#include "probeManager.hh"

#endif // PROBEMANAGER_H
