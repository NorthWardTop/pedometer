#include "common.h"
#include "display.h"

/**
 * 将console设备映射到frambuffer设备
 * @con - console设备名
 * @fb - frambuffer设备名
 */
void display_con2fbmap(const char *con, const char *fb)
{
    struct fb_con2fbmap con2fb;
    int fbfd;
    int err;
    struct stat fb_stat, con_stat;

    err = stat(fb, &fb_stat);
    err |= stat(con, &con_stat);
    if (err < 0) {
        fprintf(stderr, "stat: get device status failed: %s", strerror(errno));
        exit(-errno);
    }

    fbfd = open(fb, O_RDWR);
    if (fbfd < 0) {
        fprintf(stderr, "open: open %s device failed: %s\n", fb, strerror(errno));
        exit(-errno);
    }

    con2fb.console = minor(con_stat.st_rdev);
    con2fb.framebuffer = minor(fb_stat.st_rdev);
    err = ioctl(fbfd, FBIOPUT_CON2FBMAP, &con2fb);
    if (err < 0) {
        fprintf(stderr, "ioctl: FBIOPUT_CON2FBMAP failed: %s\n", strerror(errno));
        exit(-errno);
    }

    close(fbfd);
}


/**
 * 打开tty设备并返回一个tty终端设备描述符
 * @tty - tty设备名
 * @return - tty文件描述符
 */
int display_dev_init(const char *tty)
{
    int ttyfd;

    ttyfd = open(tty, O_WRONLY);
    if (ttyfd < 0) {
        fprintf(stderr, "open %s device error:%s", tty, strerror(errno));
        exit(-errno);
    }

    return ttyfd;
}


/**
 * 全屏显示所有信息, 共三行, 每行10字符
 * @fd - 显示设备文件描述符
 * @curr - 当前三轴加速度
 * @step - 当前步数
 * @return - 返回错误码
 */
int display_full(const int fd, const struct accel* curr, const unsigned int step)
{
    float x, y, z;
    char full[MAX_CHAR_LEN];
    int err;
    
    x = curr->ax / ACCEL_SCALE_FACTOR;
    y = curr->ay / ACCEL_SCALE_FACTOR;
    z = curr->az / ACCEL_SCALE_FACTOR;

    sprintf(full, "%-5.2f%5.2f\n%-5.2fm^2/s\nstep:%5d", x, y, z, step);
    
#ifdef __DEBUG
    fprintf(stdout, "%s\n", full);
#endif

    err = write(fd, full, strlen(full));
    if (err < 0) {
        fprintf(stderr, "writing error: %s\n", strerror(errno));
        err = -errno;
    }

    return err;
}



