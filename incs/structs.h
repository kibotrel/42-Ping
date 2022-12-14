#pragma once

# include "macros.h"
# include "stdheaders.h"

// Holds start and end of the program timestamp.
typedef struct s_time {
  // Timestamp at which the ping process began (in microseconds).
  suseconds_t start;
  // Timestamp at which the ping process stopped (in microseconds).
  suseconds_t end;
  // Timestamp at which the last packet was sent.
  suseconds_t last_sent;
} t_time;

typedef struct s_rtt {
  // Minimum time observed for packet round trip time.
  suseconds_t min;
  // Maximum time observed for packet round trip time.
  suseconds_t max;
  // Sum of all packets round trip time.
  suseconds_t total;
  // The square sum of all packets round trip time used for mdev standard deviation.
  suseconds_t square_total;
  // The exponential weighted moving average displayed on SIGQUIT.
  suseconds_t ewma;
} t_rtt;

// Holds various stats on the current ping process.
typedef struct s_stats {
  // Holds start and end of the program timestamp.
  t_time time;
  // Holds various stats about the round trip time.
  t_rtt rtt;
  // Amount of packets that were successfully received.
  uint32_t received_packets;
  // Amount of packets that were sent.
  uint32_t sent_packets;
} t_stats;

// Used to manage current ping configuration, stats and much more.
typedef struct s_ping {
  // Socket connected to the destination address.
  struct sockaddr_in destination;
  // Structure that holds various stats regarding the current ping process.
  t_stats stats;
  // CLI flags that can alter program's behaviour.
  uint32_t flags;
  // Time in seconds between each packet being sent.
  int32_t delay;
  // File descriptor of the socket.
  int32_t socket_fd;
  // Optional number of ping to do before quitting the program.
  uint64_t count;
  // Identifier to put in packets to state the right owner / receiver of the packet.
  uint16_t uid;
  // The current packet that is being processed.
  uint16_t sequence;
  // Default amount of hops the packet can do to reach the destination.
  uint8_t ttl;
  // Program's input address to ping.
  char *destination_address;
  // IP of the address to ping.
  char resolved_ip[INET6_ADDRSTRLEN];
  // DNS of the address to ping.
  char resolved_dns[MAX_DNS_SIZE];
} t_ping;
