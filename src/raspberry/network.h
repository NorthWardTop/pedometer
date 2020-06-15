#ifndef __NETWORK_H__
#define __NETWORK_H__


#include "common.h"

#define SERVER_PORT 1234


int udp_create();
void udp_recvfrom_print(int sockfd);
ssize_t udp_recv(int sockfd, struct accel *pacc);



#endif