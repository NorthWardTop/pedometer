
CC=gcc
CCFLAG=-g -Wall
TARGET=pedometer
SRC=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRC))
# OBJS=main.o network.o pace.o display.o  daemon.o

# 当make pi=y时候, 编译链更改为arm
ifeq ($(pi), y)
	CC=arm-linux-gcc
endif

# 当make ap=y时候, 编译链改为
ifeq ($(ap), y)
	CC=mipsel-openwrt-linux-gcc
	CCFLAG += -static
endif


$(TARGET):$(SRC)
	$(CC) $(CCFLAG) -o $@ $^


.PHONY:
clean:
	$(RM) $(OBJS) $(TARGET)


# 二进制程序上传到PI
uppi:
	scp $(TARGET) pi@raspberrypi.lan:/home/pi/pedometer

# 二进制程序上传到AP
upap:
	scp $(TARGET) admin@192.168.123.1:/etc/storage/bin

# 二进制上传到腾讯vps
upvps:
	scp $(TARGET) root@123.206.82.143:/root/pedometer


