import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
d = np.loadtxt("data.txt",delimiter=",")
maxt = int(np.max(d[:,0]))
print(maxt)
ims = []
fig = plt.figure()
plt.ylim(0,50)
plt.xlim(0,50)
mindp = np.min(d[:,3])
maxdp = np.max(d[:,3])
for i in range(maxt):
    mask = d[:,0] == i
    ims.append([plt.scatter(d[mask,1],d[mask,2],c=d[mask,3]),])
    plt.clim(mindp,maxdp)
plt.colorbar()
im_ani = animation.ArtistAnimation(fig, ims, interval=20, repeat_delay=1000,blit=True)
plt.show()
