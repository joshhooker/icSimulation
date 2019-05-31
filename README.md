
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
./mmSim config.json -t N
```
where N is the number of threads to run for multi-threading.

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
  "numberGrids"         : 4,
  "gridSize"            : 20,
  "gridRadius"          : 70,
  "gridDistance"        : 70,
  "scintDistance"       : 140,
  "wireThickness"       : 0.02,
  "useInches"           : false,
  "scintResolution"     : 0.1,
  "extraGridResolution" : 0.1
```

An explanation of each configuration parameter is described below:

- "interactive": When this is set to true, it will load up a GUI to visualize the detector setup and geometry. This can be only set to true or false.
- "macroName": This is macro file to run automatically when "interactive" is false. It's parameters are described above.
- "gasType": This is the name of the gas to use in the ionization chamber. Currently there are four gases to choose from: “P10” (Argon 90% CH4 10%), “CH4”, “CO2” and “CF4”. Other gases can be added by editing the MMDetectorConstruction files.
- "gasTemperature": This is temperature of the above gas in Kelvin.
- "gasPressure": This is the pressure of the above gas in Torr.
- "numberGrids": This is the total number of cathode-anode-cathode grids to use in the setup.
- "gridSize": This is the total distance from cathode to cathode. Cathode to anode distance is half of this value. This can be in mm or inches and a parameter below will describe this.
- "gridRadius": This is the radius of the grids assuming they are circular. This can be in mm or inches and a parameter below will describe this.
- "gridDistance": This is the distance from the window (entrance of the ionization chamber) to the middle of the IC grids. This can be in mm or inches and a parameter below ("useInches") will describe this.
- "scintDistance": This is the distance from the window (entrance of the ionization chamber) to the scintillator. This can be in mm or inches and a parameter below ("useInches") will describe this.
- "useInches": When this is set to true, "gridDistance", "gridRadius" and "distanceScint" will be in inches. When it is false, they will be in mm.
- "scintResolution": This is the resolution of the scintillator resolution (1 is 100%).
- "extraGridResolution": Just using the fanoFactor will still under estimate the errors of the energy in the grids. This is an extra resolution on top of that (1 is 100%).

### Output Files
Currently, the GEANT4 simulation is not available to be run with multi-threading. So as of right now, there will but one output file called "sim.root". In this file, there will be up to two TTrees depending on the "writeAllEvents" flag. The TTrees are:
```
simData
```
The simData Tree has the following branches:
```
gridEnergy_count
gridEnergy [vector]
gridID_count
gridID [vector]
scintXPosition_count 
scintXPosition [vector]
scintYPosition_count
scintYPosition [vector]
scintEnergy_count
scintEnergy [vector]
scintTrackID_count
scintTrackID [vector]
scintMass_count
scintMass [vector]
scintCharge_count
scintCharge
beamEnergy
beamCharge
beamMass
energy
cmEnergy
vertex
qValue
excitedEnergy
lightAngleCM
lightAngleLab
lightEnergy
heavyAngleCM
heavyAngleLab
heavyEnergy
lightRecoilCharge
lightRecoilMass
heavyRecoilCharge
heavyRecoilMass
```

- "gridEnergy": This is a vector and its length is determined by the "numberGrids" parameter in the config.json file. This is the energy deposited (with resolution) in each individual grid.
- "gridID": This is a vector and its length is determined by the "numberGrids" parameter in the config.json file. This is the id of each individual grid.
- "scintXPosition": This is a vector. The x-position of the particle(s) in the scintillator.
- "scintYPosition": This is a vector. The y-position of the particle(s) in the scintillator.
- "scintEnergy": This is a vector. The energy measured of the particle(s) in the scintillator (with resolution).
- "scintTrackID": This is a vector. The Track IDs of the particle(s) in the scintillator. 1 is the original beam, higher are reaction products.
- "scintMass": This is a vector. The mass of the particle(s) depositing energy in the scintillator.
- "scintCharge": This is a vector. The charge of the particle(s) depositing energy in the scintillator.
- "beamEnergy": The energy of the incoming beam.
- "beamCharge": The charge of the incoming beam.
- "beamMass": The mass of the incoming beam.
- "energy": The energy of the beam at the reaction point.
- "cmEnergy": The center of mass energy at the reaction point.
- "vertex": The vertex at the reaction point.
- "qValue": The q-value of the reaction.
- "excitedEnergy": The excited energy of the recoil from the reaction.

The light and heavy refer to the recoil products. This is mainly used to check the kinematics of the reaction.

#### Example Plots

Here are some example plots from the current settings. The run.mac and config.json used to generate these plots are located in examplePlots/.

The current beam settings are a 7Be secondary beam with contaminants of 7Li, 6Li and 4He. It is setup to do 7Be(d, n)8B. Below, the light and heavy recoils are the neutrons and 8B respectively.

Total Grid Energy vs Scintillator Energy:
![ICTotalE_ScintE](https://github.com/joshhooker/icSimulation/blob/master/examplePlots/icTotalEnergy_ScintE.png)

Light Recoil Energy vs Light Recoil Angle (for reaction products only):
![lightEnergyLab_lightAngleLab](https://github.com/joshhooker/icSimulation/blob/master/examplePlots/lightEnergy_lightAngle.png)

Heavy Recoil Energy vs Heavy Recoil Angle (for reaction products only):
![heavyEnergyLab_heavyAngleLab](https://github.com/joshhooker/icSimulation/blob/master/examplePlots/heavyEnergy_heavyAngle.png)