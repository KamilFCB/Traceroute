#include "utils.h"

typedef struct Result
{
	char time[6];
	bool host_reached;
	bool received_packets;
	char* ip_addrs;
} Result;


Result receive_packets(int *sockfd, int ttl, int pid, struct timeval *send_time);

