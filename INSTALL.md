## Dependencies

There are a number of mandatory dependencies for the minimum build which consists of the core library and the command-line query tool. These are also required and sufficient for the GEMC plugin:

* python (for build system)
* C++ compiler with C++11/C++0x support
* Boost C++ headers and program_options library
* MySQL
* CCDB

Dependencies which are needed for the Clara service include the following libraries with their associated header files:

* Clara
* cMsg
* CToolBox
* EVIO
* expat
* Boost C++ filesystem and system libraries

The unit tests require everything above, plus one additional dependency:

* Boost C++ unit_test_framework library

## Building

The build system chosen is waf which is an executable python script included in the base directory of the CLAS12 Detector Geometry Library (CDGL) project. The build is a three step process: configure, build and install. Usually, you can combine the build and install into a single command:

```sh
./waf configure --prefix=/path/to/prefix --includes=/path/to/include --libpath=/path/to/lib
./waf build install
```

The --includes and --libpath options are useful if you have installed any of the dependencies in non-standard locations that your compiler doesn't know about a priori. By default, the libpath is populated by the environment variable LD_LIBRARY_PATH (linux), PATH (windows) or DYLD_LIBRARY_PATH (mac). You can change the compiler using the CXX environment variable:

```sh
CXX=clang++ ./waf configure
```

For more information look at the output of the --help option:

```sh
./waf --help
```
