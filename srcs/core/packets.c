#include "core.h"
#include "display.h"
#include "utils.h"

uint16_t compute_checksum(uint16_t *data, int32_t size) {
  // This the standard implementation described in
  // RFC 1071. Source: https://www.rfc-editor.org/rfc/rfc1071.

  uint32_t sum = 0U;

  while (size > 1) {
    sum += *data++;
    size -= sizeof(uint16_t);
  }

  if (size > 0) {
    sum += *(uint8_t*)data;
  }

  while (sum >> sizeof(uint16_t) * 8) {
    sum = (sum & 0xffffU) + (sum >> (sizeof(uint16_t) * 8U));
  }

  return (uint16_t)(~sum);
}

void craft_icmp_message(struct icmphdr *packet, uint16_t sequence) {
  packet->type = ICMP_ECHO;
  packet->code = 0U;
  packet->un.echo.id = ft_htons(ping.uid);
  packet->un.echo.sequence = ft_htons(sequence);
  packet->checksum = compute_checksum((uint16_t *)packet, ICMP_PAYLOAD_SIZE + ICMP_HEADER_SIZE);
}

void craft_ip_header(struct iphdr *ip, uint32_t destination) {
  ip->version = IP_VERSION;
  ip->protocol = IPPROTO_ICMP;
  ip->ttl = ping.ttl;
  ip->daddr = destination;
  
  // IP Header length in blocks of 4 bytes (words). Dividing by two is
  // the same as right bitshift once. So dividing by 4 is equivalent to
  // right bitshift twice.
  ip->ihl = IP_HEADER_SIZE >> 2;
  // Total size of the packet.
  ip->tot_len = ft_htons(PACKET_SIZE);

  ip->check = compute_checksum((uint16_t *)ip, IP_HEADER_SIZE);
}

void send_packet(int32_t socket, void *packet, const struct sockaddr *destination) {
  if (sendto(socket, packet, PACKET_SIZE, 0, destination, sizeof(*destination)) < 0) {
    exit_with_code(FAILED_SENT_PACKET, INTERNAL_ERROR, NULL);
  }

  ping.stats.sent_packets++;
  ping.stats.time.last_sent = get_timestamp();
}

void receive_packet(uint32_t socket, struct sockaddr_in address, void *packet) {
  char buffer[512] = {};

  struct iovec vector = {
    .iov_base = packet,
    .iov_len = PACKET_SIZE
  };
  struct msghdr header = {
    .msg_name = &address,
    .msg_namelen = sizeof(address),
    .msg_iov = &vector,
    .msg_iovlen = 1,
    .msg_control = buffer,
    .msg_controllen = sizeof(buffer),
    .msg_flags = 0
  };

  if (recvmsg(socket, &header, 0) < 0) {
    exit_with_code(FAILED_RECEIVE_PACKET, INTERNAL_ERROR, NULL);
  }

  if (ping.flags & VERBOSE) {
    print_packet_informations(packet, "Received packet");
  }
}

void verify_packet(void *packet) {
  struct iphdr *ip = packet;
  struct icmphdr *message = packet + IP_HEADER_SIZE;
  uint16_t packet_sequence = ft_ntohs(message->un.echo.sequence);

  if (message->type == ICMP_ECHO) {
    return ;
  }

  if (message->type != ICMP_ECHOREPLY) {
    char *error = resolve_icmp_type(message->type);

    fprintf(
      stderr, "From \x1B[32m%s\x1B[0m (\x1B[32m%s\x1B[0m) icmp_seq=\x1B[33m%hu\x1B[0m %s\n",
      ping.resolved_dns, ping.resolved_ip, packet_sequence, error
    );
  } else {
    suseconds_t now = get_timestamp();
    suseconds_t rtt = now - ping.stats.time.last_sent;

    print_packet_results(ip->tot_len, packet_sequence, ip->ttl, rtt, now);
    update_ping_stats(rtt, packet_sequence);
    
    ping.stats.received_packets++;

    if (ping.flags & COUNT && ping.count == (uint64_t)ping.stats.received_packets) {
      ping_exit(0);
    }
  }
}
