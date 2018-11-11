with open("out.txt","r") as f:
    data = f.read()
import matplotlib.pyplot as plt
import matplotlib.animation as animation
SplitData = [[[float(x) for x in y.split(",")[:-1]] for y in t.split(";")[:-1]] for t in data.split("---")[:-1]]
images = []
fig = plt.figure()
for t in SplitData:
    images.append([plt.imshow(t)])
    plt.clim(0,1)
ani = animation.ArtistAnimation(fig, images, interval=5, blit=True)
plt.show()