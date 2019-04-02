/*
	Kamil Woś
	289039
*/

#include "receive.h"
#include "utils.h"


Result receive_packets(int *sockfd, int ttl, int pid, struct timeval *send_time)
{
	Result result = {{0}, false, false, NULL};
	int packets_received = 0;
	int ready = 1;
	char* ip_addrs[3] = {"0", "0", "0"};
	struct timeval receive_time[3];

	struct sockaddr_in sender;
	socklen_t sender_len = sizeof(sender);
	u_int8_t buffer[IP_MAXPACKET];

	fd_set descriptors;
	FD_ZERO(&descriptors);
	FD_SET(*sockfd, &descriptors);
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while(packets_received < 3 && ready > 0)
	{
		ready = select(*sockfd+1, &descriptors, NULL, NULL, &tv);

		if(ready < 0)
		{
			fprintf(stderr, "select error: %s\n", strerror(errno));
			return result;
		}
		else if(ready > 0)
		{
			ssize_t packet_len = recvfrom(*sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len);
		
			if(packet_len < 0)
			{
				fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
				return result;
			}
			else
			{
				char sender_ip_str[20];
				inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));

				struct iphdr* ip_header = (struct iphdr*)buffer;
				u_int8_t* icmp_packet = buffer + 4 * ip_header->ihl;
				struct icmphdr* icmp_header = (struct icmphdr*)icmp_packet;
			
				if(icmp_header->type == ICMP_ECHOREPLY)
				{
					if(validate_packet(icmp_header, pid, ttl))
					{
						ip_addrs[packets_received] = sender_ip_str;
						result.host_reached = true;
						gettimeofday(&receive_time[packets_received], NULL);
						packets_received++;
					}
				}
				else if(icmp_header->type == ICMP_TIME_EXCEEDED)
				{
					icmp_packet += 8;
					struct iphdr* packet_expired = (struct iphdr*)icmp_packet;
					icmp_packet += (packet_expired->ihl * 4);
					struct icmphdr* icmp_data = (struct icmphdr*) icmp_packet;
					
					if(validate_packet(icmp_data, pid, ttl))
					{
						ip_addrs[packets_received] = sender_ip_str;
						gettimeofday(&receive_time[packets_received], NULL);
						packets_received++;
					}
				}
			}
		}
	}

	if(packets_received < 3)
		sprintf(result.time, "%s", "???");
	else
		sprintf(result.time, "%dms", calculate_latency(send_time, &receive_time[0]));

	if(packets_received == 0)
		result.ip_addrs = "*";
	else
	{
		result.ip_addrs = ip_concat(ip_addrs);
		result.received_packets = true;
	}


	return result;
}
