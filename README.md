# Load balancer for ALICE
In collaboration with the Amsterdam University of Applied sciences and CERN a load balancer for the ALICE project.
This program is part of my thesis.

## Table of contents
* [Introduction](#introduction)
* [License](#license)
* [Compilation and installation](#compilation-and-installation)



## Introduction
```
  ALICE (A Large Ion Collider Experiment) [1] is a general purpose, heavy ion collision detector at the
CERN LHC [2]. It is designed to study the physics of strongly interacting matter, and in particular
the properties of Quark-Gluon Plasma (QGP), using proton-proton, nucleus-nucleus and proton-nucleus
collisions at high energies. The ALICE experiment will be upgraded during the Long Shutdown 2 (LS2)
in order to exploit the full the scientific potential of the future LHC.
(Technical Design Report for the Upgrade of the Online–Offline Computing System, The Alice Collaboration, June 2 2014, p. 1)
```
Due the impediment upgrade of the LHC, a new load balancer is required. This project has the following scope:
1. Make sure the First Level Processors (FLPs) target the same Event Processing Nodes(EPNs)
1. Make sure that FLPs skip defect/offline EPNs

## License 
The license is [GPL version 3](LICENSE.md), for a more up to date version or information please refer to the [AliceO2 group](https://github.com/AliceO2Group).

## Compilation and installation
This program requires you to install [FairRoot](https://fairroot.gsi.de/). Just follow the regular installation instructions.
The proper configuration for [FairSoft](https://github.com/FairRootGroup/FairSoft) is to enable debug without optimization and geant4.
Since FairSoft comes with it's own version of Boost, this program will set the boost root to use the same as FairSoft.

