#ifndef __DISPLAY_H__
#define __DISPLAY_H__



#define MAX_CHAR_LEN             30   /* 屏幕允许显示的最大字符数 */
#define OUTPUT_CON               "/dev/tty1"  /* 输出的终端设备 */
#define OUTPUT_FB                "/dev/fb1"    /* 输出的显示设备 */


void display_con2fbmap(const char *con, const char *fb);
int display_dev_init(const char *tty);
int display_full(const int fd, const struct accel* curr, const unsigned int step);


#endif









