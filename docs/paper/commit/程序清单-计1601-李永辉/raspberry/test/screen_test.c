#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>


int main(int argc, char const *argv[])
{
	int fbfd = -1;
	int stdoutfd = dup(STDOUT_FILENO);

	fbfd = open("/dev/fb1", O_WRONLY);

	dup2(fbfd, STDOUT_FILENO);
	printf("testetettststettsetsetse");
	// dup2(stdoutfd, STDOUT_FILENO);
	// close(fbfd);
	


	return 0;
}
