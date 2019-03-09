
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

The current example run.mac file looks like this:
```
/run/printProgress 1000
/run/beamOn 50000
```

This will run 50,000 events and print the progress every 1,000 events.

#### config.json

The config.json is the main file you will be editing. You can change some (not all) of the geometry, gas, reaction products and resolution parameters.

The current example config.json file looks like this:
```
{
  "interactive"         : false,
  "macroName"           : "run.mac",
  "gasType"             : "P10",
  "gasTemperature"      : 293.15,
  "gasPressure"         : 40,
  "workFunction"        : 28.0,
  "fanoFactor"          : 0.17,
  "qValue"              : -2.09,
  "target"              : [1,2],
  "lightProduct"        : [0,1],
  "heavyProduct"        : [5,8],
  "numberGrids"         : 4,
  "gridDistance"        : 20.,
  "gridRadius"          : 70,
  "distanceScint"       : 140,
  "useInches"           : false,
  "scintResolution"     : 0.1,
  "extraGridResolution" : 0.1,
  "writeAllEvents"      : true
}
```

An explanation of each configuration parameter is described below:

- "interactive": When this is set to true, it will load up a GUI to visualize the detector setup and geometry. This can be only set to true or false.
- "macroName": This is macro file to run automatically when "interactive" is false. It's parameters are described above.
- "gasType": This is the name of the gas to use in the ionization chamber. Currently there are four gases to choose from: “P10” (Argon 90% CH4 10%), “CH4”, “CO2” and “CF4”. Other gases can be added by editing the MMDetectorConstruction files.
- "gasTemperature": This is temperature of the above gas in Kelvin.
- "gasPressure": This is the pressure of the above gas in Torr
- "workFunction": This is the average energy (in eV) required to create an electron-ion pair in the gas. The Work Function is dependent on the gas type used.
- "fanoFactor": The Fano Factor defines the energy resolution for collisions not being purely statistical. The Fano Factor is dependent on the gas type used.
- "qValue": This is the Q-Value of the reaction.
- "target": Defined the be the [charge, mass] of the target nuclei.
- "lightProduct": The [charge, mass] of the light recoil.
- "heavyProduct": The [charge, mass] of the heavy recoil.
- "numberGrids": This is the total number of cathode-anode-cathode grids to use in the setup.
- "gridDistance": This is the total distance from cathode to cathode. Cathode to anode distance is half of this value. This can be in mm or inches and a parameter below will describe this.
- "gridRadius": This is the radius of the grids assuming they are circular. This can be in mm or inches and a parameter below will describe this.
- "distanceScint": This is the distance from the window (entrance of the ionization chamber) to the scintillator. This can be in mm or inches and a parameter below will describe this.
- "useInches": When this is set to true, "gridDistance", "gridRadius" and "distanceScint" will be in inches. When it is false, they will be in mm.
- "scintResolution": This is the resolution of the scintillator resolution (1 is 100%).
- "extraGridResolution": Just using the fanoFactor will still under estimate the errors of the energy in the grids. This is an extra resolution on top of that (1 is 100%).
- "writeAllEvents": When this is set to true, it will create a second TTree called "allData". This will be discussed below. This can be only set to true or false.


The Work Function and Fano Factors for the currently available gases are:
* P10 gas: workFunction is 28.0 eV and fanoFactor is 0.17
* CH4 gas: workFunction is 29.1 eV  and fanoFactor is 0.26
* CO2 gas: workFunction is 34.2 eV and fanoFactor is 0.33
* CF4 gas: workFunction is 33.8 eV and fanoFactor is ~0.2

### Output Files
Currently, the GEANT4 simulation is not available to be run with multi-threading. So as of right now, there will but one output file called "sim.root". In this file, there will be up to two TTrees depending on the "writeAllEvents" flag. The TTrees are:
```
simData
allData
```

The simData TTree has the following branches:
```
gunEnergy
icGridEnergy
icGridTotalEnergy
scintE
scintMass
scintCharge
scintXPos
scintYPos
scintZPos
```
- "gunEnergy": The energy of the incoming beam.
- "icGridEnergy": This is a vector and its length is determined by the "numberGrids" parameter in the config.json file. This is the energy deposited (with resolution) in each individual grid.
- "icGridTotalEnergy": The summed energy of all of the grids.
- "scintE": The energy measured in the scintillator (with resolution).
- "scintMass": The mass of the particle depositing energy in the scintillator.
- "scintCharge": The charge of the particle depositing energy in the scintillator.
- "scintXPos": The x-position of the particle in the scintillator.
- "scintYPos": The y-position of the particle in the scintillator.
- "scintZPos": The z-position of the particle (beam axis) in the scintillator. This should be removed and was for testing purposes.

The allData Tree has the following branches:
```
lightAngleLab
lightEnergyLab
heavyAngleLab
heavyEnergyLab
```
The light and heavy refer to the recoil products. This is mainly used to check the kinematics of the reaction.

#### Example Plots