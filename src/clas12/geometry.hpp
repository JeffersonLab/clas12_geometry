#ifndef __CLAS12_GEOMETRY_HPP__
#define __CLAS12_GEOMETRY_HPP__

/**

\mainpage CLAS12 Geometry library

This project consists of two distinct libraries:
    -# A collection of classes and objects used to describe the
        geometry of the CLAS12 detector
    -# A dynamically-loaded server interface for these geometry
        classes to be used as a service in the CLARA framework.

\section sec_geom CLAS12 Geometry and the Constants Databse

The geometry classes currently include only the drift chamber
which is entire encapsulated in the clas12::geometry::DriftChamber
class. This is the only class the end-user has access to. It
accesses the CCDB "CLAS12 constants database" to obtain the
nominal geometry parameters as well as the alignment and calibration
adjustments.

This is an example of creating an instance of the DriftChamber
geometry class, retrieving the nominal geometry parameters from the
CCDB and printing out some information.
\include examples/doc/drift_chamber_01.cpp

Here is a snippet of the above example:
\snippet examples/doc/drift_chamber_01.cpp const ref to sub object

\section sec_server CLAS12 Geometry Server



**/

#include "geometry/drift_chamber.hpp"
#include "geometry/forward_tof.hpp"

#endif /* __CLAS12_GEOMETRY_HPP__ */
