#include "core.h"
#include "display.h"
#include "utils.h"

void ping_target(__unused int signal) {
  uint8_t packet[PACKET_SIZE] = {0};

  ping.sequence++;

  craft_ip_header((void *)packet, ping.destination.sin_addr.s_addr);
  craft_icmp_message((void *)(packet + IP_HEADER_SIZE), ping.sequence);
  send_packet(ping.socket_fd, packet, (const struct sockaddr *)&ping.destination);

  if (ping.flags & VERBOSE) {
    print_packet_informations(packet, "  Sent packet  ");
  }

  if (ping.delay < 1) {
    exit_with_code(DELAY_TOO_SMALL, REGULAR_ERROR, NULL);
  }
  alarm(ping.delay);
}

void ping_exit(__unused int signal) {
  double loss = 0.0;
  suseconds_t time = get_timestamp() - ping.stats.time.start;

  if (ping.stats.sent_packets) {
    loss = (1.0 - (double)(ping.stats.received_packets / ping.stats.sent_packets)) * 100.0;
  }

  close(ping.socket_fd);
  print_exit_stats(loss, time);
  exit(EXIT_SUCCESS);
}

void current_stats(__unused int signal) {
  double loss = 0.0;

  if (ping.stats.sent_packets) {
    loss = (1.0 - (double)(ping.stats.received_packets / ping.stats.sent_packets)) * 100.0;
  }

  print_current_stats(loss);
}