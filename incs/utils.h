#pragma once

# include "core.h"

/*
**  utils/errors.c
*/

// Prints a custom message to stderr then exit with a unique error code.
void exit_with_code(uint8_t code, uint8_t error_type, char *extra);
// Get ICMP type description.
char *resolve_icmp_type(uint8_t code);
// Warn a custom message to stderr for non-fatal errors.
void warn_with_code(uint8_t code);

/*
** utils/network.c
*/

// Converts a short value from host to network endianness.
uint16_t ft_htons(uint16_t data);
// Converts a short value from network to host endianness.
uint16_t ft_ntohs(uint16_t data);
// Update current ping statistics with packet round trip time.
void update_ping_stats(suseconds_t rtt, uint16_t sequence);

/*
**  utils/time.c
*/

// Returns current timestamp in microseconds.
suseconds_t get_timestamp(void);
// Compute round trip time of a packet from its sent timestamp.
suseconds_t compute_rtt(struct timeval *time);

/*
**  utils/maths.c
*/

// Returns the maximum value between two timestamps.
suseconds_t ft_max(suseconds_t a, suseconds_t b);
// Returns the minimum value between two timestamps.
suseconds_t ft_min(suseconds_t a, suseconds_t b);
