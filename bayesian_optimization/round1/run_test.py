import json
import matplotlib.pyplot as plt
import numpy as np
import os
import shutil
import sys
from tqdm import tqdm

from analysis import analysis

def run_test():
  file = open('output.log', 'w')

  pressures = [20, 40, 60, 80, 100, 120, 140, 160, 180, 200]
  num_grids = [3]
  scint_distances = [120, 140, 160, 180, 200, 220, 240]

  for i in pressures:
    for j in num_grids:
      for k in scint_distances:
        print(i, j, k)
        run_simulation(1, i, j, k)

def run_simulation(gas_type, pressure, num_grid, scint_distance):
  file = open('output.log', 'a+')

  make_json_file(gas_type, pressure, num_grid, scint_distance)

  cmd = '../build/mmSim config.json'
  os.system(cmd)

  efficiency = analysis('sim.root', 'output.root')

  parameter_names = str(gas_type) + '_' + str(pressure) + '_' + str(num_grid) + '_' + str(scint_distance)
  dir_name = 'test/' + str(parameter_names)

  try:
    os.mkdir(dir_name)
  except:
    pass

  shutil.move('config.json', str(dir_name) + '/config.json')
  shutil.move('sim.root', str(dir_name) + '/sim.root')
  shutil.move('output.root', str(dir_name) + '/output.root')

  file.write('%s %s %s %s %s\n' % (gas_type, pressure, num_grid, scint_distance, efficiency))
  file.close()

def make_json_file(gas_type, pressure, num_grid, scint_distance):
  try:
    os.remove('config.json')
  except:
    pass

  gridDistance = 20. # mm
  data = {}
  data['interactive'] = False
  data['macroName'] = 'run.mac'

  if gas_type == 1:
    data['gasType'] = 'P10'
  elif gas_type == 2:
    data['gasType'] = 'CO2'
  elif gas_type == 3:
    data['gasType'] = 'CF4'
  else:
    data['gasType'] = 'P10'

  data['gasTemperature'] = 293.15
  data['gasPressure'] = pressure
  data['numberGrids'] = num_grid
  data['gridDistance'] = 20
  data['gridRadius'] = 70
  data['distanceScint'] = scint_distance
  data['wireThickness'] = 0.02
  data['useInches'] = False
  data['scintResolution'] = 0.1
  data['extraGridResolution'] = 0.1

  with open('config.json', 'w') as outfile:
    json.dump(data, outfile, indent=4)

  #shutil.move('config.json', '../build/config.json')

if __name__ == '__main__':
  run_test()