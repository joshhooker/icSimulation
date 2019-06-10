from bayes_opt import BayesianOptimization
from bayes_opt.event import Events
from bayes_opt.observer import JSONLogger
from bayes_opt.util import load_logs
import json
import matplotlib.pyplot as plt
import numpy as np
import os
import shutil
import sys
from tqdm import tqdm

sys.path.insert(0, '../aux_scripts/')

from analysis import analysis

counter = 0

def run_bayesian_optimization():
  file = open('output_raw.log', 'w+')
  file.close()

  # Bounds for Pressure (10 - 140 torr) and Scintillator Distance (120 - 240 mm)
  # pbounds = {'x': (10, 140), 'y': (120, 240), 'g': (1, 3), 'w': (1, 5)}
  pbounds = {'x': (10, 140), 'y': (120, 240), 'g': (1, 4), 'w': (1, 5), 'gz': (20, 200)}

  # Bayesian Optimizer
  # Verbose = 0: Silent
  # Verbose = 1: Prints only when a maximum is observed
  # Verbose = 2
  optimizer = BayesianOptimization(f=black_box, pbounds=pbounds, verbose=2, random_state=42)

  # For saving progress
  logger = JSONLogger(path="./logs.json")
  optimizer.subscribe(Events.OPTMIZATION_STEP, logger)

  # n_iter: How many steps of bayesian optimization you want to perform
  # init_points: How many steps of random exploration you want to perform
  optimizer.maximize(init_points=80, n_iter=60)

  for i, res in enumerate(optimizer.res):
    print("Iteration {}: \n\t{}".format(i, res))

  print(optimizer.max)

# x Pressure
# y Scintillator Distance
# gz Grid Center Distance
# g Gas Type
# w Number of Grids
def black_box(x, y, g, w, gz):
  a = int(g)
  b = int(w)

  print(x, y, g, w, gz, gz + w*20./2., gz - w*20./2.)

  if(gz + w*20./2. > y):
    gz = y - w*20./2. - 5.
    print("new gz = ", gz)

  if(gz - w*20./2. < 0):
    gz = w*20./2. + 5.
    print("new gz = ", gz)


  return black_box_discrete_params(x, y, gz, a, b)

# x Pressure
# y Scintillator Distance
# z Grid Center Distance
# a Gas Type
# b Number of Grids
def black_box_discrete_params(x, y, z, a, b):
  assert type(a) == int
  assert type(b) == int

  silhouette_score, efficiency = run_simulation(a, x, b, z, y)
  return silhouette_score*2. + efficiency

def run_simulation(gas_type, pressure, num_grid, grid_distance, scint_distance):
  global counter
  file = open('output_raw.log', 'a+')

  make_json_file(gas_type, pressure, num_grid, grid_distance, scint_distance)

  cmd = '../build/mmSim config.json -t 6'
  os.system(cmd)

  silhouette_score, efficiency = analysis('sim.root', 'output.root')

  dir_name = 'output/' + str(counter)

  try:
    os.mkdir(dir_name)
  except:
    pass

  shutil.move('config.json', str(dir_name) + '/config.json')
  # shutil.move('sim.root', str(dir_name) + '/sim.root')
  shutil.move('output.root', str(dir_name) + '/output.root')

  try:
    os.remove('sim.root')
  except:
    pass

  file.write('%s %s %s %s %s %s %s\n' % (gas_type, pressure, num_grid, grid_distance, scint_distance, silhouette_score, efficiency))
  file.close()

  counter += 1
  return silhouette_score, efficiency

def make_json_file(gas_type, pressure, num_grid, grid_distance, scint_distance):
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
  elif gas_type == 4:
    data['gasType'] = 'CH4'
  else:
    data['gasType'] = 'P10'

  data['gasTemperature'] = 293.15
  data['gasPressure'] = pressure
  data['numberGrids'] = 3
  data['gridSize'] = 20
  data['gridRadius'] = 70
  data['gridDistance'] = grid_distance
  data['scintDistance'] = scint_distance
  data['wireThickness'] = 0.02
  data['useInches'] = False
  data['scintResolution'] = 0.1
  data['extraGridResolution'] = 0.1

  with open('config.json', 'w') as outfile:
    json.dump(data, outfile, indent=4)

if __name__ == '__main__':
  run_bayesian_optimization()
