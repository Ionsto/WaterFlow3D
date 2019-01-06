import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation
d = np.loadtxt("out.txt",delimiter=",")
maxt = int(np.max(d[:,0]))
print(maxt)
ims = []
fig = plt.figure()
ax = Axes3D(fig)
plt.ylim(0,20)
plt.xlim(0,20)
mindp = np.min(d[:,4])
maxdp = np.max(d[:,4])
mindp = -1
maxdp = 1
for i in range(0,maxt,10):
    mask = d[:,0] == i
    ims.append([ax.scatter(d[mask,1],d[mask,3],d[mask,2],c=d[mask,4]),])
    ims.append([ax.scatter(d[mask,1],d[mask,3],d[mask,2],c=d[mask,4]),])
    #ax.clim(mindp,maxdp)
#plt.colorbar()
im_ani = animation.ArtistAnimation(fig, ims, interval=20, repeat_delay=1000,blit=True)
plt.show()
