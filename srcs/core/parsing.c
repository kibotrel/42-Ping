#include "core.h"
#include "utils.h"

static char *load_address_information(char *address) {
  struct addrinfo *results;
  struct addrinfo hints = { .ai_family = AF_INET };

  if (address == NULL) {
    exit_with_code(MISSING_ADDRESS, REGULAR_ERROR, NULL);
  }

  if (getaddrinfo(address, NULL, &hints, &results)) {
    exit_with_code(UNKNOWN_ADDRESS, REGULAR_ERROR, address);
  }

  ping.destination.sin_addr.s_addr =
    ((struct sockaddr_in *)results->ai_addr)->sin_addr.s_addr;
  freeaddrinfo(results);

  // This case is for 0.0.0.0, The default behaviour would be to ping
  // any address of the network, in this case localhost. The iputils-ping
  // implementation fallbacks this address to 127.0.0.1 in order to ping
  // a single address as it does for all other addresses. Source: https://shorturl.at/eGQT8.
  if (!ping.destination.sin_addr.s_addr) {
    if (getaddrinfo("127.0.0.1", NULL, &hints, &results)) {
      exit_with_code(UNKNOWN_ADDRESS, REGULAR_ERROR, "127.0.0.1");
    }

    ping.destination.sin_addr.s_addr =
      ((struct sockaddr_in *)results->ai_addr)->sin_addr.s_addr;
    freeaddrinfo(results);

    return ("127.0.0.1");
  }

  return (address);
}

char *parse_cli_options(int argc, char **argv) {
  int8_t option;
  int64_t ttl;
  int64_t count;

  while ((option = getopt(argc, argv, "c:Dhi:qt:v")) != -1) {
    switch (option){
      case 'c':
        ping.flags |= COUNT;
        count = strtoul(optarg, NULL, 10);

        if (count <= 0 || count > INT64_MAX) {
          exit_with_code(COUNT_NOT_POSITIVE, REGULAR_ERROR, optarg);
        }

        ping.count = (uint64_t)count;
        break;
      case 'D':
        ping.flags |= TIMESTAMP;
        break;
      case 'i':
        ping.flags |= INTERVAL;
        ping.delay = strtol(optarg, NULL, 10);
        break;
      case 'q':
        ping.flags |= QUIET;
        break;
      case 't':
        ping.flags |= TTL;
        ttl = strtol(optarg, NULL, 10);

        if (ttl < 0 || ttl > UINT8_MAX) {
          exit_with_code(TTL_NOT_IN_RANGE, REGULAR_ERROR, optarg);
        }

        ping.ttl = (uint8_t)ttl;
        break;
      case 'v':
        ping.flags |= VERBOSE;
        break;
      default: exit_with_code(DISPLAY_USAGE, REGULAR_ERROR, NULL);
    }
  }

  // optind is a static variable used by getopt() to indicate which
  // the argument is being processed by the function. At the end it holds
  // the value of the first index that isn't a flag. 
   return (load_address_information(argv[optind]));
}
