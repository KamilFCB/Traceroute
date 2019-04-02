/*
	Kamil Woś
	289039
*/

#include "send.h"

void send_packet(int *sockfd, int ttl, int pid, char* ip)
{
	struct icmphdr icmp_header;
	icmp_header.type = ICMP_ECHO;
	icmp_header.code = 0;
	icmp_header.un.echo.id = pid;
	icmp_header.un.echo.sequence = 1899+ttl;
	icmp_header.checksum = 0;
	icmp_header.checksum = compute_icmp_checksum((u_int16_t*)&icmp_header, sizeof(icmp_header));
	struct sockaddr_in recipient;

	bzero (&recipient, sizeof(recipient));
	recipient.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &recipient.sin_addr);

	setsockopt(*sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));

	
	ssize_t bytes_sent = sendto(*sockfd, &icmp_header, sizeof(icmp_header), 0, (struct sockaddr*)&recipient, sizeof(recipient));
		
	if (bytes_sent < 0)
		fprintf(stderr, "Packet sending error: %s\n", strerror(errno));



}
