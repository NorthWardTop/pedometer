#ifndef __PEDOMETER_H__
#define __PEDOMETER_H__


#include "common.h"


#define FILTER_CNT                      4       /* 均值滤波器容器大小 */
#define SAMPLE_SIZE                     50      /* 动态峰值样品容器大小 */
#define DYNAMIC_PRECISION               1500      /* 动态精度 两次变化差的绝对值决定是否更新 */

#define AXIS_NULL                       '\0'
#define AXIS_X                          'x'
#define AXIS_Y                          'y'
#define AXIS_Z                          'z'
#define ACTIVE_PRECISION      			60       /*活跃轴最小变化值*/

#define TIME_INTERVAL_MIN               200     /* 最小时间间隔 0.2s */
#define TIME_INTERVAL_MAX               2000    /* 最大时间间隔 2s */

// 平均滤波器容器
struct filter_avg {
	struct accel info[FILTER_CNT];
	unsigned char count;
};


//峰值结构, 存储上一个峰值和新峰值
struct peak_val {
    struct accel new_max, new_min;
    struct accel old_max, old_min;
};


// 一个线性移位寄存器，用于过滤高频噪声
struct slid_reg {
    struct accel new_sample;
    struct accel old_sample;
};


// 时间间隔
struct time_interval {
    struct timeval last;
    struct timeval this;
};


void filter_init(struct filter_avg *filter);
void filter_calculate(const struct filter_avg *filter, struct accel *sample);

void peak_val_init(struct peak_val *peak);
void peak_val_update(struct peak_val *peak, const struct accel *curr_val);

void slid_init(struct slid_reg *slid);
char slid_update(struct slid_reg *slid, const struct accel *curr_val);

void time_init(struct time_interval *differ);
void time_update(struct time_interval *differ);

char time_cond(struct time_interval *differ);
char space_cond(const struct peak_val *peak, const struct slid_reg *slid);

#endif

