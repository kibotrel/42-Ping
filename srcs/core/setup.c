#include "utils.h"

int create_raw_socket(void) {
  int fd;
  int options = 1;

  // Create a raw socket processing IPv4 ICMP packets.
  // You could also use AF_INET as domain.
  // More info: https://stackoverflow.com/a/6737450.
  fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);

  if (fd < 0) {
    exit_with_code(FAILED_SOCKET_CREATION, INTERNAL_ERROR, NULL);
  }

  // Set socket options to force the inclusion of an IP header to send
  // packets. More info on this StackOverflow topic: https://shorturl.at/jpx16.
  if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &options, sizeof(int)) < 0) {
    exit_with_code(SET_SOCKET_OPTIONS, INTERNAL_ERROR, NULL);
  }

  return fd;
}
