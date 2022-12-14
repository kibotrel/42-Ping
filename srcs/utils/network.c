#include "utils.h"

uint16_t ft_htons(uint16_t data) {
  int32_t little_endian = 1;

  if (*(uint8_t *)&little_endian) {
    return (((data & 0xffU) << 8U) | ((data & 0xff00U) >> 8U));
  } else {
    return (data);
  }
}

uint16_t ft_ntohs(uint16_t data) {
  int32_t little_endian = 1;

  if (*(uint8_t *)&little_endian) {
    return (((data & 0xffU) << 8U) | ((data & 0xff00U) >> 8U));
  } else {
    return (data);
  }
}

void update_ping_stats(suseconds_t rtt, uint16_t sequence) {
  ping.stats.rtt.min = sequence == 1 ? rtt : ft_min(ping.stats.rtt.min, rtt);
  ping.stats.rtt.max = sequence == 1 ? rtt : ft_max(ping.stats.rtt.max, rtt);
  ping.stats.rtt.total += rtt;
  ping.stats.rtt.square_total += rtt * rtt;
  
  if (ping.stats.rtt.ewma) {
    ping.stats.rtt.ewma += rtt - ping.stats.rtt.ewma / 8;
  } else {
    ping.stats.rtt.ewma = rtt * 8;
  }
}
