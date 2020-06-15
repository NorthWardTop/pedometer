
#include "common.h"
#include "pace.h"


/**
 * 初始化平均过滤器容器
 * @filter - 容器
 */
void filter_init(struct filter_avg *filter)
{
    bzero(filter, sizeof(struct filter_avg));
}


/**
 * 求一个样品容器内的平均
 * @filter - 样品容器
 * @sample - 返回的平均值
 */
void filter_calculate(const struct filter_avg *filter, struct accel *sample)
{
    unsigned int i;
	int x_sum = 0, y_sum = 0, z_sum = 0;

    //求一个样品容器内的平均
	for (i = 0; i < FILTER_CNT; i++) {
		x_sum += filter->info[i].ax;
		y_sum += filter->info[i].ay;
		z_sum += filter->info[i].az;
	}
	sample->ax = x_sum / FILTER_CNT;
	sample->ay = y_sum / FILTER_CNT;
	sample->az = z_sum / FILTER_CNT;
}


/**
 * 初始化峰值结构
 *      - 将最大峰值设置位最小值
 *      - 最小峰值设置位最大值, 能够准确被立即更新掉
 * @peak - 峰值结构
 */
void peak_val_init(struct peak_val *peak)
{
    peak->new_max.ax = INT16_MIN;
    peak->new_max.ay = INT16_MIN;
    peak->new_max.az = INT16_MIN;
    // peak->old_max.ax = INT16_MIN;
    // peak->old_max.ay = INT16_MIN;
    // peak->old_max.az = INT16_MIN; 

    peak->new_min.ax = INT16_MAX;
    peak->new_min.ay = INT16_MAX;
    peak->new_min.az = INT16_MAX;
    // peak->old_min.ax = INT16_MAX;
    // peak->old_min.ay = INT16_MAX;
    // peak->old_min.az = INT16_MAX;  
}


/**
 * 更新动态峰值, SAMPLE_SIZE次更新一次清空
 * @peak - 峰值结构
 * @curr_val - 传入当前值
 */
void peak_val_update(struct peak_val *peak, const struct accel *curr_val)
{
    static unsigned int sample_cnt = 0;
    sample_cnt++;
    if (sample_cnt > SAMPLE_SIZE) {
        sample_cnt = 1;
        peak->old_max = peak->new_max;
        peak->old_min = peak->new_min;
        peak_val_init(peak);
    }

    peak->new_max.ax = max(peak->new_max.ax, curr_val->ax);
    peak->new_max.ay = max(peak->new_max.ay, curr_val->ay);
    peak->new_max.az = max(peak->new_max.az, curr_val->az);

    peak->new_min.ax = min(peak->new_min.ax, curr_val->ax);
    peak->new_min.ay = min(peak->new_min.ay, curr_val->ay);
    peak->new_min.az = min(peak->new_min.az, curr_val->az);
}


/**
 * 初始化动态精度
 * @slid - 动态精度
 */
void slid_init(struct slid_reg *slid)
{
    bzero(slid, sizeof(struct slid_reg));
}


/**
 * 根据新值和精度范围确定是否更新
 * @slid - 过滤器
 * @current_val - 当前值
 * @return - 是否更新
 */
char slid_update(struct slid_reg *slid, const struct accel *curr_val)
{
    char seted = 0;

    //无论如何, old值一直更新为new值
    slid->old_sample = slid->new_sample;
    
    //如果当前值与过滤器值差值超过阀值, 就更新过滤器
    if (abs(curr_val->ax - slid->new_sample.ax) > DYNAMIC_PRECISION) {
        slid->new_sample.ax = curr_val->ax;
        seted = 1;
    }

    if (abs(curr_val->ay - slid->new_sample.ay) > DYNAMIC_PRECISION) {
        slid->new_sample.ay = curr_val->ay;
        seted = 1;
    }

    if (abs(curr_val->az - slid->new_sample.az) > DYNAMIC_PRECISION) {
        slid->new_sample.az = curr_val->az;
        seted = 1;
    }


    
    return seted;
}


/**
 * 判断最活跃的轴
 * @peak - 峰值结构
 * @return - 返回最活跃的轴
 */
static char active_axis(const struct peak_val *peak)
{
    char axis = AXIS_NULL;
    int16_t x_change = abs(peak->new_max.ax - peak->new_min.ax);
    int16_t y_change = abs(peak->new_max.ay - peak->new_min.ay);
    int16_t z_change = abs(peak->new_max.az - peak->new_min.az);

    if (x_change == max3(x_change, y_change, z_change)) 
        axis = AXIS_X;
    else if (y_change == max3(x_change, y_change, z_change))
        axis = AXIS_Y;
    else if (z_change == max3(x_change, y_change, z_change))
        axis = AXIS_Z;

    return axis;
}


/**
 * 初始化时间间隔, 原始间隔初始化位 1s
 * @differ - 时间间隔
 */
void time_init(struct time_interval *differ)
{
    bzero(differ, sizeof(struct time_interval));
    gettimeofday(&differ->last, NULL);
    gettimeofday(&differ->this, NULL);
    differ->this.tv_sec++;
}


/**
 * 更新时间间隔, 新时间位当前时间
 * @differ - 时间间隔
 */
void time_update(struct time_interval *differ)
{
    struct timeval new;

    gettimeofday(&new, NULL);
    differ->last = differ->this;
    differ->this = new;
}


/**
 * 根据时间间隔, 判断是否有效步数
 * @differ - 上步与这步时间差
 * @return - 是否有效步
 */
char time_cond(struct time_interval *differ)
{
    
    long last_ms = differ->last.tv_sec * 1000 + differ->last.tv_usec / 1000;
    long this_ms = differ->this.tv_sec * 1000 + differ->this.tv_usec / 1000;
    long interval = this_ms - last_ms;
    char step = 0;
    
    
    // 两次时间间隔在时间窗口内, 不在则清空时间间隔重新计算时间
    if (interval > TIME_INTERVAL_MIN && interval < TIME_INTERVAL_MAX)
        step = 1;
    else 
        time_init(differ);
    
    return step;
}


/**
 * 检查计步的空间条件
 *      - 先获取到活跃轴
 *      - 当新老样本在峰值平均的两侧, 就说明有一个周期
 * @peak - 峰值
 * @slid - 经低通滤波处理过的样本
 */
char space_cond(const struct peak_val *peak, const struct slid_reg *slid)
{
    char step = 0;
    char axis = active_axis(peak); //获取最活跃的轴

    switch (axis) {
		case AXIS_NULL: {
			break;
		}
		case AXIS_X: {
			int16_t threshold_x = (peak->old_max.ax + peak->old_min.ax) / 2;
			if (slid->old_sample.ax > threshold_x && slid->new_sample.ax < threshold_x) 
				step = 1;
			break;
		}
		case AXIS_Y: {
			int16_t threshold_y = (peak->old_max.ay + peak->old_min.ay) / 2;
			if (slid->old_sample.ay > threshold_y && slid->new_sample.ay < threshold_y) 
				step = 1;
			break;
		}
		case AXIS_Z: {
			int16_t threshold_z = (peak->old_max.az + peak->old_min.az) / 2;
			if (slid->old_sample.az > threshold_z && slid->new_sample.az < threshold_z) 
				step = 1;
			break;
		}
		default: 
			break;
	}
    
    return step;
}

