# CLAS12 Detector Geometry Library

## Authors:

* John T. Goetz <goetz@jlab.org>
* Yelena Prok <yprok@jlab.org>

The CLAS12 Detector Geometry Library (CDGL) consists of a single core library which is used by three provided interfaces:

* Command-line query tool (clas12geom),
* GEMC (Geant4 Monte Carlo) simulator plugin,
* Clara (CLAS Reconstruction and Analysis Framework) service.

The underlying database uses the CCDB scheme borrowed (unchanged) from the Glue-X software project. A minimal set of parameters describing the detector is stored in the database, along with misalignment and warping parameters. These are read in by the CDGL which calculates the requested values (drift-chamber wire end-points for example) and returns these in different formats depending on the receiver: XML for command-line and Clara, in-memory volume map for GEMC.

Please see the [GitHub Pages](http://jeffersonlab.github.io/clas12_geometry) associated with this project for more documentation and API references.

See the [INSTALL.md file](INSTALL.md) for details on how to build, test, run and
generate reference documentation for this project on your system.

See the [COPYING file](COPYING) for licensing and copyright information.

```
Copyright 2014 John T. Goetz, Yelena Prok
```
