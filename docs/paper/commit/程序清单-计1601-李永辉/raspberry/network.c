

#include "common.h"
#include "network.h"


/**
 * 创建一个UDP套接字
 * @return - 返回socket描述符
 */
int udp_create()
{
    int sockfd;
    struct sockaddr_in addr;
    int ret=0;

    //udp, 数据包套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "Socket error: %s\n", strerror(errno));
        exit(1);
    }

    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port=htons(SERVER_PORT);

    ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        fprintf(stderr, "Socket error: %s\n", strerror(errno));
        close(sockfd);
        exit(1);
    }

    return sockfd;
}


/**
 * 只打印接受数据, 不保存数据
 * @sockfd - socket描述符
 * @return - void
 */
void udp_recvfrom_print(int sockfd)
{
    unsigned int addrlen;
    int bytes=0;
    struct sockaddr_in addr;
    struct raw_data data;

    fprintf(stdout, "Waiting client data...\n");

    while (1) {
        bzero(&data, sizeof(data));
        addrlen = sizeof(struct sockaddr); 
        // bytes = recvfrom(sockfd, msg, 128, 0, (struct sockaddr*)&addr, &addrlen);
        bytes = recvfrom(sockfd, &data, sizeof(data), 0, (struct sockaddr*)&addr, &addrlen);
        // msg[bytes] = 0;


        // fprintf(stdout, "from client [%s:%hu] say: \n%s\n\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), msg);

        fprintf(stdout, "[%s:%hu]: Ax:%hd  Ay:%hd  Az:%hd  Gx:%hd  Gy:%hd  Gz:%hd  T:%hd  size:%d\n", \
            inet_ntoa(addr.sin_addr), ntohs(addr.sin_port),
            data.acc.ax, data.acc.ay, data.acc.az, data.gyr.gx, data.gyr.gy, data.gyr.gz, data.t, bytes);
    }
}



/**
 * @sockfd - 传入socket描述符
 * @pacc - 加速度数据指针, 用于返回加速度结构体值
 * @return - 返回读取字节数或者错误返回-1
 */
ssize_t udp_recv(int sockfd, struct accel *pacc)
{
    int bytes = 0;
    struct raw_data data;

    bytes = recv(sockfd, &data, sizeof(data), 0);
    if (bytes < 0) {
        fprintf(stderr, "Receive error: %s", strerror(errno));
        return -1;
    }
    //给传入的结构体赋值, 改变加速度值
    *pacc = data.acc;

    return bytes;
}

