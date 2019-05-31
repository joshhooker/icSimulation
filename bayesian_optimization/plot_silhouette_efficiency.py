import matplotlib.pyplot as plt
import numpy as np
import scipy.interpolate

cm = plt.cm.get_cmap('RdYlBu_r')

data = np.loadtxt('output_raw.log')

x = data[:, 1]
y = data[:, 3]
z = data[:, 4]

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