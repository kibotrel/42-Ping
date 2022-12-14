#pragma once

# include "structs.h"
# include "enums.h"

# define __unused __attribute__((unused))

extern t_ping ping;

/*
** core/packets.c
*/

// Compute a RFC 1071 compliant checksum for packets.
uint16_t compute_checksum(uint16_t *data, int32_t size);
// Craft an IPv4 header to send a packet.
void craft_ip_header(struct iphdr *ip, uint32_t destination);
// Craft an ICMP Echo request packet.
void craft_icmp_message(struct icmphdr *packet, uint16_t sequence);
// Send a packet to another machine.
void send_packet(int32_t socket, void *packet, const struct sockaddr *destination);
// Receive a packet from another machine.
void receive_packet(uint32_t socket, struct sockaddr_in address, void *packet);
// Verify a received packet in order to print out some details.
void verify_packet(void *packet);

/*
** core/parsing.c
*/

// Parse command line options and arguments to alter the program behaviour accordingly.
char *parse_cli_options(int argc, char **argv);

/*
** core/setup.c
*/


// Create and setup a raw socket using the ICMP protocol.
int create_raw_socket(void);

/*
** core/signals.c
*/

// Send an ICMP Echo request to the target on SIGALRM signal.
void ping_target(__unused int signal);
// Display ping statistics and exit on SIGINT signal.
void ping_exit(__unused int signal);
// Display on-going ping statistics on SIGQUIT
void current_stats(__unused int signal);