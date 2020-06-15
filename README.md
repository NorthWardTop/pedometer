# remote-step-monitor
基于树莓派的远程(NodeMCU)计步监控(MPU6050)


<!-- 1.熟悉树莓派开发板的硬件构成和软件开发流程；
2.学习加速度传感器的基本工作原理和典型应用；
3.阅读Digilent公司PmodACL2的用户手册，掌握其接口和数据通信原理；
4.选择合适的软硬件模块搭建一个能够实现远程计步器监控的软硬件系统，其中计步终端采集三轴运动信息，并定时将该信息发送给树莓派主机，树莓派主机进行数据分析，计算并显示步数。 -->

# 任务书

任务书做了小的改动：三轴加速度计选择了一个更低成本的
1. 熟悉树莓派开发板的硬件构成和软件开发流程； 
2. 学习加速度传感器的基本工作原理和典型应用； 
3. 选择一种三轴加速度传感器，掌握其接口和数据通信原理； 
4. 选用合适的软硬件模块搭建一个能够实现远程计步器监控的软硬件系统，其中计步终端采集三轴运动信息，并定时将该信息发送给树莓派主机；树莓派主机进行数据分析，计算并显示步数。 
5. 分析加速度传感器的三轴数据，实现计步算法。


# 硬件选型以及连接方式
1. 
'''shell
上位机: raspberry Pi 3 Model B V1.2
||
局域网
|| 
无线通信: NodeMCU (ESP8266) 
||
I2C传输
||
传感器: MPU6050
'''

2. MPU6050-NodeMCU
VCC-3V
GND-G
SCL-D1
SDA-D2(NodeMCU默认D1D2为I2C)

# 软件框架选择和库导入

## NodeMCU: 使用Arduion框架, 用到的库代码有
1. AsyncUDP - Arduion自带, 用于异步发送UDP数据
2. I2Cdev - 后期导入, 用于NodeMCU的I2C设备操作, 
        头文件和源文件分别位于/src/nodemcu-arduino-asyncudp/include和/src/nodemcu-arduino-asyncudp/src
3. MPU6050 - 后期导入, 封装了MPU6050一些操作, 
        头文件和源文件分别位于/src/nodemcu-arduino-asyncudp/include和/src/nodemcu-arduino-asyncudp/src

## Raspberry Pi: 使用2019-09-26-raspbian-buster-lite操作系统
1. 使用Linux API 创建UDP服务器并接受NodeMCU发送的数据
2. 

# 遇到的问题

## see /doc/problem.md



# 参考文献
//加速度传和陀螺仪介绍
//http://www.starlino.com/imu_guide.html

//计步算法
//https://www.analog.com/cn/analog-dialogue/articles/pedometer-design-3-axis-digital-acceler.html
