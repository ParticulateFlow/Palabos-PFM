/*

Palabos case timer

A simple header file to keep track of time durations. Uses ctime to
take snapshots. Takes an MPI communicator to make sure times are only
taken on processor 0, thus only works in settings where MPI is enabled
in Palabos.

Palabos case timer is free software released under the GPLv3

author: Philippe Seil (philippe.seil@jku.at)

 */


#ifndef PLB_CASE_TIMER_H
#define PLB_CASE_TIMER_H

#include "core/globalDefs.h"
#include "parallelism/mpiManager.h"
#include "io/parallelIO.h"

#include <mpi.h>
#include <ctime>
#include <vector>
#include <map>
#include <ostream>

#include "plbCaseTimerException.h"

namespace plb {
  class PlbCaseTimer{
  public:
    PlbCaseTimer();
  
    void startGlobal();
    void stopGlobal();

    void registerCustom(char const *name);
    void startCustom(char const *name);
    void stopCustom(char const *name);

    void writeLog(Parallel_ostream &out);
    void writeLogJSON(std::string fname);

  private:
    static int const GLOBAL_IND = 0;
    static double const CPS = CLOCKS_PER_SEC;
    typedef std::map<std::string,int> index_map_t;
    typedef index_map_t::value_type index_t;

    index_map_t indexMap;

    std::vector<clock_t> clockVec;
    std::vector<clock_t> timeVec;
    std::vector<bool> activeVec;

    void start(int const index);
    void stop(int const index);
    int getIndex(char const *name_);
    double computeOther();
  };


}; // namespace plb

#include "plbCaseTimer.hh"

#endif // PLB_CASE_TIMER_H
