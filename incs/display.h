#pragma once

# include "core.h"

// Print IP and ICMP header informations of the given packet.
void print_packet_informations(void *packet, char source[]);
// Print ICMP Echo Reply packets stats.
void print_packet_results(uint16_t packet_size, uint16_t sequence, uint8_t ttl, suseconds_t rtt, suseconds_t now);
// Print global ping statistics on program's exit.
void print_exit_stats(double loss, suseconds_t time);
// Print current ping statistics.
void print_current_stats(double loss);