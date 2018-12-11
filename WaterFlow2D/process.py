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
for i in range(maxt):
    mask = d[:,0] == i
    ims.append([plt.scatter(d[mask,1],d[mask,2],c="b"),])
im_ani = animation.ArtistAnimation(fig, ims, interval=50, repeat_delay=3000,blit=True)
plt.show()
