#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>  
#include <sys/param.h>
#include <arpa/inet.h>
#include <linux/limits.h>
#include <linux/kernel.h> //å¯¼å…¥max() min()
#include <linux/fb.h>
#include <signal.h>

// #define __DEBUG

#define ACCEL_SCALE_FACTOR		16384.0
#define GYRO_SCALE_FATOR		131.0



struct accel
{
    int16_t ax, ay, az;
};

struct gyro
{
    int16_t gx, gy, gz;
};


struct raw_data {
    struct accel acc;
    struct gyro gyr;
    int16_t t;
};



/* Indirect macros required for expanded argument pasting, eg. __LINE__. */
#define ___PASTE(a,b) a##b
#define __PASTE(a,b) ___PASTE(a,b)
#define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)
/*
 * min()/max()/clamp() macros that also do
 * strict type-checking.. See the
 * "unnecessary" pointer comparison.
 */
#define __min(t1, t2, min1, min2, x, y) ({		\
	t1 min1 = (x);					\
	t2 min2 = (y);					\
	(void) (&min1 == &min2);			\
	min1 < min2 ? min1 : min2; })

/**
 * min - return minimum of two values of the same or compatible types
 * @x: first value
 * @y: second value
 */
#define min(x, y)					\
	__min(typeof(x), typeof(y),			\
	      __UNIQUE_ID(min1_), __UNIQUE_ID(min2_),	\
	      x, y)

#define __max(t1, t2, max1, max2, x, y) ({		\
	t1 max1 = (x);					\
	t2 max2 = (y);					\
	(void) (&max1 == &max2);			\
	max1 > max2 ? max1 : max2; })

/**
 * max - return maximum of two values of the same or compatible types
 * @x: first value
 * @y: second value
 */
#define max(x, y)					\
	__max(typeof(x), typeof(y),			\
	      __UNIQUE_ID(max1_), __UNIQUE_ID(max2_),	\
	      x, y)

/**
 * min3 - return minimum of three values
 * @x: first value
 * @y: second value
 * @z: third value
 */
#define min3(x, y, z) min((typeof(x))min(x, y), z)

/**
 * max3 - return maximum of three values
 * @x: first value
 * @y: second value
 * @z: third value
 */
#define max3(x, y, z) max((typeof(x))max(x, y), z)



#endif