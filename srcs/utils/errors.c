#include "core.h"


const char* const error_messages[] = {
  [SUCCESS] = "No problem",
  [NO_ROOT_PERMISSION] = "You need root permissions to run this program.",
  [DISPLAY_USAGE]  = "\nUsage\n  ft_ping [options] <destination>\n\nOptions:\n  <destination>      dns name or ip address\n  -c <count>         stop after <count> replies\n  -D                 print timestamps\n  -h                 print help and exit\n  -q                 quiet output\n  -t <ttl>           define time to live\n  -v                 verbose output",
  [MISSING_ADDRESS] = "\x1B[31;1musage\x1B[0m error: Destination address required",
  [FAILED_SOCKET_CREATION] = "Socket file descriptor couldn't be obtained",
  [SET_SOCKET_OPTIONS] = "Failed to set socket options",
  [UNKNOWN_ADDRESS] = "Name or service not known",
  [FAILED_SENT_PACKET] = "Failed to send packet to target",
  [FAILED_GET_TIME] = "Failed to get time",
  [FAILED_SIGNAL_HOOK] = "Failed to hook function on signal",
  [COUNT_NOT_POSITIVE] = "out of range: \x1B[33m1\x1B[0m <= value <= \x1B[33m9223372036854775807\x1B[0m",
  [DELAY_TOO_SMALL] = "minimal interval allowed for used is \x1B[33m1\x1B[0ms",
  [TTL_NOT_IN_RANGE] = "out of range \x1B[33m0\x1B[0m <= value <= \x1B[33m255\x1B[0m"
};

void exit_with_code(uint8_t code, uint8_t type, char *extra) {
  if (code == DISPLAY_USAGE) {
    fprintf(stderr, "%s\n", error_messages[DISPLAY_USAGE]);
  } else if (type == INTERNAL_ERROR) {
    fprintf(stderr, "\x1B[31;1mInternal error:\x1B[0m %s\n", error_messages[code]);
  } else if (code == UNKNOWN_ADDRESS) {
    fprintf(stderr, "ft_ping: \x1B[32m%s\x1B[0m: %s\n", extra, error_messages[UNKNOWN_ADDRESS]);
  } else if (code == COUNT_NOT_POSITIVE || code == TTL_NOT_IN_RANGE) {
    fprintf(stderr, "ft_ping: \x1B[31;1minvalid argument:\x1B[0m '\x1B[32m%s\x1B[0m' %s\n", extra, error_messages[code]);
  } else if (code == DELAY_TOO_SMALL) {
    fprintf(stderr, "ft_ping: \x1B[31;1mcannot flood\x1B[0m; %s\n", error_messages[DELAY_TOO_SMALL]);
  } else {
    fprintf(stderr, "ft_ping: %s\n%s\n", error_messages[code], error_messages[DISPLAY_USAGE]);
  }

  exit(code);
}

void warn_with_code(uint8_t code) {
  fprintf(stderr, "ft_ping: \x1B[30;1mwarning\x1B[0m: %s\n", error_messages[code]);
}

char *const icmp_types_messages[] = {
  [ICMP_DEST_UNREACH] = "Destination unreachable",
  [ICMP_SOURCE_QUENCH] = "Source quench",
  [ICMP_REDIRECT] = "Redirect message",
  [ICMP_TIME_EXCEEDED] = "Time exceeded",
  [ICMP_PARAMETERPROB] = "Parameter problem",
  [ICMP_TIMESTAMP] = "Timestamp",
  [ICMP_TIMESTAMPREPLY] = "Timestamp reply",
  [ICMP_INFO_REQUEST] = "Information request",
  [ICMP_INFO_REPLY] = "Information reply",
  [ICMP_ADDRESS] = "Address mask request",
  [ICMP_ADDRESSREPLY] = "Address mask reply",
};

char *resolve_icmp_type(uint8_t code) {
  char *error = NULL;
 
  if (code < sizeof(icmp_types_messages)) {
    error = icmp_types_messages[code];
    
    return (error);
  } else {
    return (NULL);
  }
}
