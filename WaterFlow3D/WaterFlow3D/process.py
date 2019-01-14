import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
d = np.loadtxt("out.txt",delimiter=",")
maxt = int(np.max(d[:,0]))
print(maxt)
ims = []
fig = plt.figure()
plt.ylim(0,20)
plt.xlim(0,20)
mindp = np.min(d[:,4])
maxdp = np.max(d[:,4])
mindp = 0
maxdp = 2000
for i in range(0,maxt,5):
    mask = d[:,0] == i
    ims.append([plt.scatter(d[mask,1],d[mask,2],c=d[mask,4]),])
    plt.clim(mindp,maxdp)
plt.colorbar()
plt.tight_layout()
im_ani = animation.ArtistAnimation(fig, ims, interval=20, repeat_delay=1000,blit=True)
im_ani.save('im.mp4')
plt.show()
