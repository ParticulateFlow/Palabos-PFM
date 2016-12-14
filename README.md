# Palabos-PFM

This repository contains a (largely unmodified) fork of the LB code Palabos as distributed by FlowKit Ltd. via [palabos.org](http://www.palabos.org). 

Maintainer: Philippe Seil (philippe.seil@jku.at), Johannes Kepler University Linz, Austria

## Branches

The existence of three branches in this repository is guaranteed:

* **master**: contains both Palabos and additional utilities developed by PFM/JKU. Palabos is usually unmodified except
for minor bugfixes in case they arise and a few tiny issues (see next section). Utilities are found in externalLibraries/plb/utils.
* **develop**: Development version. May contain newer features, but may break.
* **plb-vanilla**: contains the unmodified content of the latest version of Palabos.

Other branches present are usually temporary feature branches and should by no means used in any productive environement.

## Changes to vanilla Palabos

### MRT dynamics

In Palabos, the relaxation rates for MRT dynamics are preset to values from literature. However, most people nowadays use either the TRT relaxation from Ginzburg et al. or the relaxation rates from Pan et al. (2006). Those two settings can now be used by defining
```
#define MRT_USE_TRT_RELAXATION
```
or
```
#define MRT_USE_PAN_RELAXATION 
```
**BEFORE** the Palabos headers are included. The switches only work in 3D.

## Utilities

* [PlbCaseTimer](externalLibraries/pfm/utils/PlbCaseTimer/README.md)
* [ProbeManager](externalLibraries/pfm/utils/ProbeManager/README.md)

