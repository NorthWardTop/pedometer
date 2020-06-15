


print("sasa")
# addr = ("192.168.1.179", 1234)
 
# while True:

#     data = "hi server"
#     s.sendto(data.encode(), addr)
#     time.sleep(1)

# s.close()

# import numpy as np
# from matplotlib.lines import Line2D
# import matplotlib.pyplot as plt
# import matplotlib.animation as animation


# class Scope(object):
#     def __init__(self, ax, maxt=2, dt=0.02):
#         self.ax = ax
#         self.dt = dt
#         self.maxt = maxt
#         self.tdata = [0]
#         self.ydata = [0]
#         self.line = Line2D(self.tdata, self.ydata)
#         self.ax.add_line(self.line)
#         self.ax.set_ylim(-.1, 1.1)
#         self.ax.set_xlim(0, self.maxt)

#     def update(self, y):
#         lastt = self.tdata[-1]
#         if lastt > self.tdata[0] + self.maxt:  # reset the arrays
#             self.tdata = [self.tdata[-1]]
#             self.ydata = [self.ydata[-1]]
#             self.ax.set_xlim(self.tdata[0], self.tdata[0] + self.maxt)
#             self.ax.figure.canvas.draw()

#         t = self.tdata[-1] + self.dt
#         self.tdata.append(t)
#         self.ydata.append(y)
#         self.line.set_data(self.tdata, self.ydata)
#         return self.line,


# def emitter(p=0.03):
#     'return a random value with probability p, else 0'
#     while True:
#         v = np.random.rand(1)
#         if v > p:
#             yield 0.
#         else:
#             yield np.random.rand(1)

# # Fixing random state for reproducibility
# np.random.seed(19680801)


# fig, ax = plt.subplots()
# scope = Scope(ax)

# # pass a generator in "emitter" to produce data for the update func
# ani = animation.FuncAnimation(fig, scope.update, emitter, interval=10,
#                               blit=True)

# plt.show()



# import socket
# import struct

# import numpy as np

# from matplotlib import pyplot as plt
# from matplotlib import animation as antt
# from mpl_toolkits.mplot3d import Axes3D

# s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# s.bind(("192.168.123.14", 1234))
# print("UDP bound on port 1234...")


# while True:
#     raw_data, addr = s.recvfrom(14)
#     # print("Receive from %s:%s" % addr)
#     data = struct.unpack("hhhhhhh", raw_data)
#     print(data)
#     print(data[0], data[1])
#     plt.show()

# s.close()




# def init():  # only required for blitting to give a clean slate.
#     line.set_ydata([np.nan] * len(x))
#     return line,
 
