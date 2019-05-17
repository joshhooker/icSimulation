import matplotlib.pyplot as plt
import numpy as np

# Wire Size in um
# Wire Spacing is 2.794 mm
wire_spacing = 2.794

num_grids = 3
num_wires = num_grids*2 + 1

wireSize = [0, 20, 50, 100, 150]
efficiency = [0.999, 0.952, 0.871, 0.774, 0.688]

expected = []
for wire in wireSize:
  wire = wire/1000.
  ratio = wire/wire_spacing
  total_ratio = ratio*num_wires
  expected.append(1. - total_ratio)

plt.scatter(wireSize, efficiency, label='Simulation')
plt.scatter(wireSize, expected, label='Expected')

plt.legend(loc=0)

plt.xlabel('Wire Size [um]')
plt.ylabel('Efficiency')

plt.show()