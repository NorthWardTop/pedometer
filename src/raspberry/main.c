
#include "common.h"
#include "daemon.h"
#include "network.h"
#include "pace.h"
#include "display.h"


int main(int argc, char const *argv[])
{
    int sockfd = 0;
    int displayfd = 0;
    unsigned int i = 0;
    struct accel acc;
    unsigned int step_conut = 0;

    struct filter_avg avg; //均值滤波器
    struct accel filter_result; //保存均值过滤结果
    struct peak_val peak; //存储峰值
    struct slid_reg slid;  //存储动态精度
    struct time_interval differ; //存储步步间时间间隔


    if (0 != geteuid()) {
        fprintf(stderr, "Must be run by root: %s\n", strerror(EACCES));
        exit(-EACCES);
    }

    init_daemon(); //守护进程的方式运行

    //算法初始化
    filter_init(&avg);
    peak_val_init(&peak);
    slid_init(&slid);
    time_init(&differ);

    // 通信初始化
    sockfd = udp_create();
    display_con2fbmap(OUTPUT_CON, OUTPUT_FB);
    displayfd = display_dev_init(OUTPUT_CON);

    //算法开始, 接受数据和处理
    for (;;) {
        
        udp_recv(sockfd, &acc);

#ifdef __DEBUG
        // fprintf(stdout, "ax:%hd  ay:%hd  az:%hd  size:%d\n", acc.ax, acc.ay, acc.az, bytes);
#endif

        avg.info[i++ % FILTER_CNT] = acc; // 循环队列存入
        filter_calculate(&avg, &filter_result); //平均过滤器进行计算

        peak_val_update(&peak, &filter_result); //同时将平均过滤器过滤结果更新到峰值

        // 判断精度值是否更新
        if (slid_update(&slid, &filter_result)) {
            //判断当前变化是否满足空间要求和时间要求
            if (time_cond(&differ) && space_cond(&peak, &slid)) {
                step_conut+=2;
                time_update(&differ); //更新上一步和这一步的时间间隔
#ifdef __DEBUG
                fprintf(stdout, "step_count: %d\n", step_conut);
#endif
            }
        }

        display_full(displayfd, &filter_result, step_conut);
    }
    
    
    return 0;
}
