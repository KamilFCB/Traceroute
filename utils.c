/*
	Kamil Woś
	289039
*/

#include "utils.h"

char* ip_concat(char** ip_addrs)
{
	char* res = ip_addrs[0];
	strcat(res, " ");

	if(ip_addrs[0] != ip_addrs[1] && strcmp(ip_addrs[1], "0"))
	{
		strcat(res, ip_addrs[1]);
		strcat(res, " ");
	}
	if(ip_addrs[0] != ip_addrs[2] && ip_addrs[1] != ip_addrs[2] && strcmp(ip_addrs[2], "0"))
	{
		strcat(res, ip_addrs[2]);
		strcat(res, " ");
	}

	return res;
}

int calculate_latency(struct timeval *send_time, struct timeval *receive_time)
{
	long long res = 0;

	for(int i = 0; i < 3; i++)
	{
		res += (receive_time[i].tv_sec - send_time->tv_sec) * 1000000 + (receive_time[i].tv_usec - send_time->tv_usec);
	}

	return (int)(res / 3 / 1000);
}

u_int16_t compute_icmp_checksum(const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert(length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

bool is_valid_ip_addr(char* ip)
{
	unsigned char buf[sizeof(struct in6_addr)];
	int res = inet_pton(AF_INET, ip, buf);

	return res > 0;
}

bool validate_packet(struct icmphdr* data, int pid, int ttl)
{
	return data->un.echo.id == pid && data->un.echo.sequence == (1899+ttl);
}
