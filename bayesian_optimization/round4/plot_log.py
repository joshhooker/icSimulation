from decimal import Decimal
import json
import matplotlib.pyplot as plt
import numpy as np
import scipy.interpolate

cm = plt.cm.get_cmap('RdYlBu')

with open('logs.json') as f:
  data = json.load(f, parse_float=np.float)

x_og = []
y_og = []
z_og = []

for i in range(len(data)):
  x_og.append(data[i]['params']['x'])
  y_og.append(data[i]['params']['y'])
  z_og.append(data[i]['target'])

x = np.asarray(x_og)
y = np.asarray(y_og)
z = np.asarray(z_og)

# Set up a regular grid of interpolation points
xi, yi = np.linspace(x.min(), x.max(), 100), np.linspace(y.min(), y.max(), 100)
xi, yi = np.meshgrid(xi, yi)

# Interpolate
rbf = scipy.interpolate.Rbf(x, y, z, function='linear')
zi = rbf(xi, yi)

plt.imshow(zi, vmin=z.min(), vmax=z.max(), origin='lower',
           extent=[x.min(), x.max(), y.min(), y.max()], cmap=cm)
plt.colorbar()
plt.scatter(x, y, s=15, facecolors='none', edgecolors='k')

plt.xlabel('Pressure [Torr]')
plt.ylabel('Window to Scintillator Distance [mm]')
plt.show()