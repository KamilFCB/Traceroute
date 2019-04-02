/*
	Kamil Woś
	289039
*/

#include "send.h"
#include "receive.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
		return EXIT_FAILURE;
	
	char* ip_addr = argv[1];

	if (!is_valid_ip_addr(ip_addr))
	{
		fprintf(stderr, "Wrong ip address\n");
		return EXIT_FAILURE;
	}

	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (sockfd < 0)
	{
		fprintf(stderr, "socket error: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	
	int pid = getpid();
	Result res;
	struct timeval send;

	for (int ttl = 1; ttl <= 30; ttl++)
	{
		gettimeofday(&send, NULL);
		
		for (int i = 0; i < 3; i++)
			send_packet(&sockfd, ttl, pid, ip_addr);

		res = receive_packets(&sockfd, ttl, pid, &send);

		if(!res.received_packets)
			printf("%d. *\n", ttl);
		else
			printf("%d. %s %s\n", ttl, res.ip_addrs, res.time);

		if(res.host_reached)
			break;
	}
	
	return 0;
}
