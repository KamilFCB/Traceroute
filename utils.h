#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

u_int16_t compute_icmp_checksum(const void *buff, int length);
char* ip_concat(char** ip_addrs);
int calculate_latency(struct timeval *send_time, struct timeval *receive_time);
bool is_valid_ip_addr(char* ip);
bool validate_packet(struct icmphdr* data, int pid, int ttl);
