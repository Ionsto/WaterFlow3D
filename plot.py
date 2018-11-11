with open("data.txt","r") as f:
    data = f.read()
import matplotlib.pyplot as plt
SplitData = [[[float(x) for x in y.split(",")[:-1]] for y in t.split(";")[:-1]] for t in data.split("---")[:-1]]
for t in SplitData:
    plt.figure()
    plt.imshow(t)
    plt.clim(0,1)
plt.show()