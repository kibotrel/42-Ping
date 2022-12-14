#include "core.h"
#include "utils.h"

t_ping ping = {
  .destination = {
    .sin_family        = AF_INET,
    .sin_port          = 0
  },
  .stats = {
    .time = {
      .start           = 0.0,
      .end             = 0.0,  
      .last_sent       = 0,
    },
    .rtt = {
      .min             = 0.0,
      .max             = 0.0,
      .total           = 0.0,
      .square_total    = 0.0,
      .ewma            = 0.0,
    },
    .received_packets  = 0U,
    .sent_packets      = 0U,
  },
  .delay               = 1,
  .count               = 0,
  .sequence            = 0U,
  .socket_fd           = 0,
  .flags               = 0U,
  .uid                 = 0U,
  .ttl                 = BASE_TTL,
  .destination_address = NULL
};

int main(int argc, char **argv) {
  if (getuid() != 0) {
    exit_with_code(NO_ROOT_PERMISSION, REGULAR_ERROR, NULL);
  } else 
  if (argc < 2) {
    exit_with_code(DISPLAY_USAGE, REGULAR_ERROR, NULL);
  } else {
    // Register callbacks to trigger when process receive specific signals.
    if (
      signal(SIGALRM, &ping_target) == SIG_ERR ||
      signal(SIGINT, &ping_exit) == SIG_ERR ||
      signal(SIGQUIT, &current_stats) == SIG_ERR
    ){
      exit_with_code(FAILED_SIGNAL_HOOK, INTERNAL_ERROR, NULL);
    }

    ping.uid = getpid();
    ping.destination_address = parse_cli_options(argc, argv);
    
    getnameinfo((void *)&ping.destination, sizeof(ping.destination),ping.resolved_dns, MAX_DNS_SIZE, NULL, 0, 0);
    inet_ntop(AF_INET, (void *)&ping.destination.sin_addr, ping.resolved_ip, INET6_ADDRSTRLEN);
    
    ping.socket_fd = create_raw_socket();
    ping.stats.time.start = get_timestamp();

    printf("\x1B[1mPING\x1B[0m \x1B[32m%s\x1B[0m (\x1B[32m%s\x1B[0m) \x1B[33m%d\x1B[0m(\x1B[33m%d\x1B[0m) bytes of data.\n", ping.destination_address, ping.resolved_ip, ICMP_PAYLOAD_SIZE, PACKET_SIZE);    
    ping_target(NO_SIGNAL);

    uint8_t packet[PACKET_SIZE] = {0};

    while (TRUE) {
      receive_packet(ping.socket_fd, ping.destination, packet);
      verify_packet(packet);
    }
  }
}
