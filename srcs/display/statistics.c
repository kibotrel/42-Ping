#include "utils.h"

static const char		*ip_format = \
"╔═════════════════╗\n" \
"║ \x1B[36;1m%s\x1B[0m ║\n" \
"╠═════════════════╩════════════════════════════════════════════════════════════╗\n" \
"║                                   \x1B[32;1mIP header\x1B[0m                                  ║\n" \
"╠═════════════╦═════════════════╦════════════╦═════════════════╦═══════════════╣\n" \
"║ Version: \x1B[33m%02u\x1B[0m ║  IHL: \x1B[33m%02u\x1B[0m words  ║  TOS: \x1B[33m%03hhu\x1B[0m  ║  TTL: \x1B[33m%03hhu\x1B[0m hops  ║ Protocol: \x1B[33m%03hhu\x1B[0m ║\n" \
"╠═════════════╩══════════╦══════╩════════════╩═══════╦═════════╩═══════════════╣\n" \
"║ Identification: \x1B[33m0x%04hx\x1B[0m ║  Header checksum: \x1B[33m0x%04hx\x1B[0m  ║ Total length: \x1B[33m%03hu\x1B[0m bytes ║\n" \
"╠════════════════════════╩════════════╦══════════════╩═════════════════════════╣\n" \
"║     Source address: \x1B[33m0x%08x\x1B[0m      ║    Destination address: \x1B[33m0x%08x\x1B[0m     ║\n" \
"╠═════════════════════════════════════╩════════════════════════════════════════╣\n";

static const char *icmp_format = \
"║                                  \x1B[32;1mICMP header\x1B[0m                                 ║\n" \
"╠═════════════════════╦═════════════════════╦══════════════════════════════════╣\n" \
"║      Type: \x1B[33m%03hhu\x1B[0m      ║      Code: \x1B[33m%03hhu\x1B[0m      ║         Checksum: \x1B[33m0x%04hx\x1B[0m         ║\n" \
"╠═════════════════════╩═════════════╦═══════╩══════════════════════════════════╣\n" \
"║      Identifier: \x1B[33m0x%08x\x1B[0m       ║        Sequence Number \x1B[33m0x%08x\x1B[0m        ║\n" \
"╚═══════════════════════════════════╩══════════════════════════════════════════╝\n";

void	print_packet_informations(void *packet, char source[]) {
	struct iphdr *ip = packet;
	struct icmphdr *icmp = packet + IP_HEADER_SIZE;

	printf(
		ip_format,
		source, ip->version, ip->ihl, ip->tos, ip->ttl, ip->protocol,
		ft_ntohs(ip->id), ip->check, ft_ntohs(ip->tot_len),
		ip->saddr, ip->daddr
	);

	printf(
		icmp_format,
		icmp->type, icmp->code, icmp->checksum,
		ft_ntohs(icmp->un.echo.id), ft_ntohs(icmp->un.echo.sequence)
	);
}

void print_packet_results(uint16_t packet_size, uint16_t sequence, uint8_t ttl, suseconds_t rtt, suseconds_t now) {
	if (!(ping.flags & QUIET)) {
		if (ping.flags & TIMESTAMP) {
			printf("[\x1B[35m%ld.%ld\x1B[0m] ", now / 1000000l, now % 1000000l);
		}

		printf(
			"\x1B[33m%hu\x1B[0m bytes from \x1B[32m%s\x1B[0m (\x1B[32m%s\x1B[0m): icmp_seq=\x1B[33m%hu\x1B[0m ttl=\x1B[33m%hhu\x1B[0m time=\x1B[33m%ld.%02ld\x1B[0m ms\n",
			(uint16_t)(ft_ntohs(packet_size) - IP_HEADER_SIZE), ping.resolved_dns, ping.resolved_ip,
			sequence, ttl, rtt / 1000l, rtt % 1000l
		);
	}
}

void print_exit_stats(double loss, suseconds_t time) {
	printf(
    "\n--- \x1B[32m%s\x1B[0m ping statistics ---\n\x1B[33m%u\x1B[0m packets transmitted, \x1B[33m%u\x1B[0m received, \x1B[33m%d\x1B[0m%% packet loss, time \x1B[33m%d\x1B[0mms\n",
    ping.destination_address, ping.stats.sent_packets, ping.stats.received_packets, (int)loss, (int)(time / 1000l)
  );

  if (ping.stats.received_packets <= 0) {
    printf("\n");
  } else {
    // Mean deviation computation: used to evaluate how far from
    // the mean each value in the set is in average. Source:
    // https://shorturl.at/vOS01.
    suseconds_t mdev = (ping.stats.rtt.total / ping.stats.received_packets);

    ping.stats.rtt.square_total /= ping.stats.received_packets;
    printf(
      "rtt min/avg/max/mdev = \x1B[33m%.3lf\x1B[0m/\x1B[33m%.3lf\x1B[0m/\x1B[33m%.3lf\x1B[0m/\x1B[33m%.3lf\x1B[0m ms\n",
      ping.stats.rtt.min / 1000.0,
      ping.stats.rtt.total / ping.stats.received_packets / 1000.0,
      ping.stats.rtt.max / 1000.0,
      sqrt(ping.stats.rtt.square_total - mdev * mdev) / 1000.0
    );
  }
}

void print_current_stats(double loss) {
	double average = 0.0;

	if (ping.stats.received_packets) {
		average = ping.stats.rtt.total / ping.stats.received_packets / 1000.0;
	}

	printf(
		"\x1B[33m%u\x1B[0m/\x1B[33m%u\x1B[0m packets, \x1B[33m%d\x1B[0m%% loss, min/avg/ewma/max = \x1B[33m%.3lf\x1B[0m/\x1B[33m%.3lf\x1B[0m/\x1B[33m%.3lf\x1B[0m/\x1B[33m%.3lf\x1B[0m ms\n",
		ping.stats.received_packets, ping.stats.sent_packets, (int)loss,
		ping.stats.rtt.min / 1000.0,
		average,
		ping.stats.rtt.ewma / 8000.0,
		ping.stats.rtt.max / 1000.0
	);
}