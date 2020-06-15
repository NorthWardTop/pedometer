#!/usr/bin/python3
# -*- coding: utf-8 -*-


import socket
import struct
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation



s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', 1234))
print("UDP bound on port 1234...")

fig, ax = plt.subplots()
plt.axis([0,200,-2,2])
# x = np.arange(-17000, 17000, 1)
x = np.linspace(0, 200, 200)
z1 = [0]*200
z2 = [0]*200
z3 = [0]*200
line1, = ax.plot(z1,'r')
line2, = ax.plot(z2,'g')
line3, = ax.plot(z3,'b')
AccelScaleFactor = 16384
GyroScaleFactor = 131


def animate(i):
    raw_data, addr = s.recvfrom(14)
    # print("Receive from %s:%s" % addr)
    data = struct.unpack("hhhhhhh", raw_data)
    print(data)
    
    # z[i%2000]=data[0]/AccelScaleFactor
    z1.pop()
    z1.insert(0,data[0]/AccelScaleFactor)
    z2.pop()
    z2.insert(0,data[1]/AccelScaleFactor)
    z3.pop()
    z3.insert(0,data[2]/AccelScaleFactor)
    # z.insert(0,data[0]/AccelScaleFactor)
    line1.set_ydata(z1)  # update the data.
    line2.set_ydata(z2)  # update the data.
    line3.set_ydata(z3)  # update the data.
    return line1,line2,line3


ani = animation.FuncAnimation(
    # fig, animate, blit=True, save_count=50)
    fig, animate,  interval=2, blit=True, save_count=50)

plt.show()

