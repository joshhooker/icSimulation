# VANDLE Ionization Chamber

GEANT4 Simulation for an ionization chamber to be used with (d, n) reactions using VANDLE (Versatile Array of Neutron Detectors at Low Energy). The current configuration includes grids for energy loss in a gas and a scintillator to be used for timing and energy.

## Getting Started

These instructions will get you a copy of the project up and running on your machine.

### Prerequisites

Required Packages:

```
GEANT4
ROOT
GSL
CMake
```

### Build and Run

To build and run the simulation with the above packages installed, create and move to a build directory

```
mkdir build && cd build
```

Use CMake to then build and make the simulation executable

```
cmake .. && make
```

To run the simulation, use the created executable with a configuration file (a sample is provided)

```
./mmSim config.json
```

### Configuration Files

There are two configuration files provided needed to run the simulation: run.mac and config.json.

#### run.mac

#### config.json
