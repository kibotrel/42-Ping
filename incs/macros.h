#pragma once

// Lowest possible size for an IP header.
# define IP_HEADER_SIZE 20
// Number of bytes allocated to the content of the packet.
# define ICMP_PAYLOAD_SIZE 56
// Bare minimum of bytes needed to create an ICMP packet header.
# define ICMP_HEADER_SIZE ICMP_MINLEN
// Lowest possible size for an ICMP packet.
# define PACKET_SIZE (IP_HEADER_SIZE + ICMP_PAYLOAD_SIZE + ICMP_HEADER_SIZE)

// Version number for IPv4.
# define IP_VERSION 4
// Hops that a packet can do before considering that the packet is lost.
# define BASE_TTL 64
// Maximum bytes length of a DNS name described in RFC 1035.
# define MAX_DNS_SIZE 255
// Offset at which you'll find the ICMP Echo request timestamp within the ICMP Echo Reply packet.
# define TIMESTAMP_OFFSET (IP_HEADER_SIZE + ICMP_HEADER_SIZE + 4)

// Used to trigger ping_target() function with a correct call. This signal isn't used anyways.
# define NO_SIGNAL 0
