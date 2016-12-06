## ProbeManager

`template<typename T, template<typename U> class Descriptor> class ProbeManager` is a class to keep track of velocity and pressure probes. 

### Use

#### Initialization

The constructor of `ProbeManager`
```
ProbeManager(std::string const &velocityFname_, 
             std::string const &densityFname_, 
             ArrayVector const &velocityProbeLocations_,
             ArrayVector const &densityProbeLocations_,
             IncomprFlowParam<T> const &param_,
             T const xFactor_ = 0, T const tFactor_ = 0, 
             T const velFactor_ = 0, T const rhoFactor_ = 0);
```

expects filenames for probe file names and `ArrayVector` (which is just `typedef std::vector<Array<T,3> > ArrayVector`) for probe locations. Additionally, `IncomprFlowParam<T>` need to be passed. Per default, the locations are expected in dimensionless units and converted to LB units inside `ProbeManager`. Output is also given in dimensionless units. If another unit system should be used, the four optional parameters `xFactor_`, `tFactor_`, `velFactor_`, `rhoFactor_` need to be given in order to ensure proper unit conversion. For example, conversion to physical units is performed via v_lb*velFactor_ = v_phys.

After creating an instance 
```
ProbeManager<T,Descriptor> pM(...)
```
the headers of the output files need to be written using
```
pM.writeHeaders()
```

#### Use during simulation

Whenever desired, the probes can be written to the files by calling
```
pM.writeVelocityProbes(lattice,iT)
pM.writeDensityProbes(lattice,iT)
```
where `lattice` is the instance of `MultiBlockLattice3D` used for the simulation and `iT` is an integer with the current iteration.

#### Cleanup

No cleanup is required upon end of simulation.
