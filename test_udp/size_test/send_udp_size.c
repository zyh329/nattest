#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include "datSize.h"

#define SIP "127.0.0.1"
#define TEST_EVERY 210 //us
#define TEST_TIMES 10000

static struct sockaddr_in local_addr, host_sin;
int sockfd;

int local_net_init(int port){
	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(port);

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
		return -1;
	}

	if(bind(sockfd, (struct sockaddr *) (&local_addr),sizeof(local_addr)) == -1){
		printf ("Bind error: %s\a\n", strerror (errno));
		return -2;
	}

	return 0;
}

void main()
{
	int i = 0;
	char data[UNIT_SIZE];
	local_net_init(1000);
	memset(&host_sin, 0, sizeof(host_sin));
	host_sin.sin_family = AF_INET;
	host_sin.sin_addr.s_addr = inet_addr(SIP);
	host_sin.sin_port = htons(1001);

	while(i < TEST_TIMES)
	{
		sendto(sockfd, data, UNIT_SIZE, 0, (struct sockaddr *)&host_sin, sizeof(host_sin));
#if TEST_EVERY
		usleep(TEST_EVERY);
#endif
		i++;
	}

	close(sockfd);
}
