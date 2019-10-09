import matplotlib.pyplot as plt
import numpy as np
with open("log.txt","r") as f:
    lines = f.readlines()
plt.figure()
meanvals = []
class Vector():
    X = 0
    Y = 0
    def __init__(self,string):
        #print(string.split(":"))
        self.X,self.Y = list(map(double,string.split(":")))
    def Mag(self):
        return self.X * self.X + self.Y * self.Y
class Particle():
    def __init__(self,string=""):
        if string != "":
            args = string.split(",")
            self.Pos = Vector(args[0])
            self.Vel = Vector(args[1])
            self.Force = Vector(args[2])
            self.Momentum = Vector(args[3])
            self.StressDX = Vector(args[4])
            self.StressDY = Vector(args[5])
            self.StrainRateDX = Vector(args[6])
            self.StrainRateDY = Vector(args[7])
frames = [[Particle(p) for p in line.split(";")[:-1]] for line in lines]
plt.figure()
plt.plot([sum((p.StressDX.X for p in frame))/len(frame) for frame in frames],label="Stress DX")
plt.plot([sum((p.StressDY.Y for p in frame))/len(frame) for frame in frames],label="Stress DX")
plt.figure()
plt.plot([sum((p.StrainRateDX.X for p in frame))/len(frame) for frame in frames],label="Stress DX")
plt.plot([sum((p.StrainRateDY.Y for p in frame))/len(frame) for frame in frames],label="Stress DX")
#plt.plot([sum((p.StressDY.Mag() for p in frame))/len(frame) for frame in frames],label="Stress DY")
#for frame in lines:
#    particles = frame.split(";")[:-1]
#    posmean = 0
#    velmean = 0
#    forcemean = 0
#    for part in particles:
#        velmean += vx * vx + vy * vy 
#    velmean /= len(particles)
#    meanvals.append(velmean)
#plt.plot(meanvals)
plt.show()
