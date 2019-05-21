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

from analysis import analysis

counter = 0

def run_bayesian_optimization():
  file = open('output_raw.log', 'w+')
  file.close()

  # Bounds for Pressure (10 - 140 torr) and Scintillator Distance (120 - 240 mm)
  pbounds = {'x': (10, 140), 'y': (120, 240), 'g': (1, 3), 'w': (1, 5)}

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
  optimizer.maximize(init_points=2, n_iter=10)

  for i, res in enumerate(optimizer.res):
    print("Iteration {}: \n\t{}".format(i, res))

  print(optimizer.max)

def black_box(x, y, g, w):
  a = int(g)
  b = int(w)

  if(w*20 > y):
    return 0

  return black_box_discrete_params(x, y, a, b)

def black_box_discrete_params(x, y, a, b):
  assert type(a) == int
  assert type(b) == int

  silhouette_score, efficiency = run_simulation(a, x, b, y)
  return silhouette_score + efficiency

def run_simulation(gas_type, pressure, num_grid, scint_distance):
  global counter
  file = open('output_raw.log', 'a+')

  make_json_file(gas_type, pressure, num_grid, scint_distance)

  cmd = '../../build/mmSim config.json'
  os.system(cmd)

  silhouette_score, efficiency = analysis('sim.root', 'output.root')

  parameter_names = str(gas_type) + '_' + str(pressure) + '_' + str(num_grid) + '_' + str(scint_distance)
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

  file.write('%s %s %s %s %s %s\n' % (gas_type, pressure, num_grid, scint_distance, silhouette_score, efficiency))
  file.close()

  counter += 1
  return silhouette_score, efficiency

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

if __name__ == '__main__':
  run_bayesian_optimization()