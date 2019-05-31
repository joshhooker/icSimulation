from matplotlib import cm
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

plt.rc('text', usetex=True)

data = np.loadtxt('output.log')
xdata = data[:, 1]
ydata = data[:, 3]
zdata = data[:, 4]

fig = plt.figure()
ax = plt.axes(projection='3d')

ax.scatter3D(xdata, ydata, zdata, c=zdata, cmap=cm.coolwarm);
ax.set_xlabel('Pressure [Torr]')
ax.set_ylabel('Distance from Scintillator to Window [mm]')
ax.set_zlabel('(d, n) $^{8}$B Efficiency')
plt.show()