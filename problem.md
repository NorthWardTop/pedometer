
## 问题1: 解决使用DMP库的远程I2C设备的读写问题(DMP不含远程socket读写I2C功能)
### 问题: 由于DMP库只有针对本地I2C设备的操作, 无法针对远程socket设备读写

### 方案1: 重定向I2C设备描述符到socket
    在NodeMCU端, 通过网路TCP/UDP收到的每字节消息, 同步读写到I2C接口, 也就是读写MPU6050设备
    在raspberrypi端, 通过虚拟一个I2C设备, 并将虚拟I2C设备重定向到socket即可实现
      那么对虚拟I2C设备的读写就相当于对socket的读写
      再启动一个进程, 正常使用DMP库, 但操作的是I2C虚拟设备

### 方案2: 不使用DMP库, 自己在树莓派端实现算法
    https://blog.csdn.net/Dancer__Sky/article/details/81504778
    https://cdc.tencent.com/2013/07/26/%e5%88%a9%e7%94%a8%e4%b8%89%e8%bd%b4%e5%8a%a0%e9%80%9f%e5%99%a8%e7%9a%84%e8%ae%a1%e6%ad%a5%e6%b5%8b%e7%ae%97%e6%96%b9%e6%b3%95/



## 问题2: 屏幕驱动问题
// 加载屏幕驱动, 无效 sudo modprobe fbtft_device name=nokia5110

/**
 * 

 Nokia5110屏幕驱动安装, 原文地址:
https://yangxuan8282.gitbooks.io/rpi_cache/chapter26.html

sudo modprobe fbtft_device name=nokia3310
con2fbmap 1 1


[  992.380245] fbtft: module is from the staging directory, the quality is unknown, you have been warned.
[  992.387489] fbtft_device: module is from the staging directory, the quality is unknown, you have been warned.
[  992.388780] spidev spi0.0: spidev spi0.0 125000kHz 8 bits mode=0x00
[  992.388795] spidev spi0.1: spidev spi0.1 125000kHz 8 bits mode=0x00
[  992.388845] bcm2708_fb soc:fb: soc:fb id=-1 pdata? no
[  992.388901] spidev spi0.0: Deleting spi0.0
[  992.389883] fbtft_device: GPIOS used by 'nokia3310':
[  992.389896] fbtft_device: 'reset' = GPIO25
[  992.389903] fbtft_device: 'dc' = GPIO24
[  992.389909] fbtft_device: 'led' = GPIO23
[  992.389925] spidev spi0.1: spidev spi0.1 125000kHz 8 bits mode=0x00
[  992.389944] spi spi0.0: fb_pcd8544 spi0.0 400kHz 8 bits mode=0x00
[  992.427833] fb_pcd8544: module is from the staging directory, the quality is unknown, you have been warned.
[  992.570449] graphics fb1: fb_pcd8544 frame buffer, 84x48, 7 KiB video memory, fps=20, spi0.0 at 0 MHz
*/