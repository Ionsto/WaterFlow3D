import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation
from matplotlib.animation import FuncAnimation

d = np.loadtxt("out.txt",delimiter=",")
maxt = int(np.max(d[:,0]))
print(maxt)
ims = []
fig = plt.figure()
ax = Axes3D(fig)
#ax.set_ylim(0,20)
#ax.set_xlim(0,1)
#ax.set_zlim(0,20)
mindp = np.min(d[:,4])
maxdp = np.max(d[:,4])
mindp = -1
maxdp = 1
def animate(i):
    mask = d[:,0] == i
    ax.scatter(d[mask,1],d[mask,3],d[mask,2],c=d[mask,4])
ani = FuncAnimation(fig,animate,frames=maxt)
plt.show()