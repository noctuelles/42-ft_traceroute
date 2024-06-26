/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:31:09 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/26 15:15:17 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "icmp.h"
#include "libft.h"
#include "socket.h"

// static int
// send_probe(int fd, int ttl, const struct sockaddr *sa, socklen_t slen) {
//     if (setsockopt(fd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) == -1) {
//         ft_error(0, 0, "getsockopt: %s", strerror(errno));
//         return (-1);
//     }
//     if (sendto(fd, PROBE_CONTENT, sizeof(PROBE_CONTENT), 0, sa, slen) == -1) {
//         ft_error(0, 0, "sendto: %s", strerror(errno));
//         return (-1);
//     }
//     return (0);
// }

// static bool
// is_icmp_for_traceroute(const struct icmp *icmp, const struct in_addr *host_addr) {
//     /* ICMP error messages contain a data section that includes a copy of the entire IPv4 header, plus at least the first eight bytes of
//      * data from the IPv4 packet that caused the error message.  */

//     const struct ip     *orig_ip  = NULL;
//     const struct udphdr *orig_udp = NULL;

//     orig_ip = (const struct ip *)icmp->icmp_dun.id_data;
//     if (orig_ip->ip_dst.s_addr != host_addr->s_addr) {
//         return (false);
//     }
//     if (orig_ip->ip_p != IPPROTO_UDP) {
//         return (false);
//     }
//     orig_udp = (const struct udphdr *)(icmp->icmp_dun.id_data + (orig_ip->ip_hl << 2));
//     if (ntohs(orig_udp->dest) != g_opts.port) {
//         return (false);
//     }
//     return (true);
// }

// static int
// recv_packet(int fd, const struct in_addr *host_addr) {
//     static uint8_t     payload[IP_MAXPACKET] = {0};
//     struct sockaddr_in sa_in                 = {0};
//     socklen_t          sa_len                = sizeof(sa_in);
//     ssize_t            payload_len           = 0;
//     struct ip         *ip                    = NULL;
//     struct icmp       *icmp                  = NULL;

//     if ((payload_len = recvfrom(fd, payload, sizeof(payload), 0, (struct sockaddr *)&sa_in, &sa_len)) == -1) {
//         ft_error(0, 0, "recvfrom: %s", strerror(errno));
//         return (-1);
//     }
//     if (icmp_packet_decode(payload, (size_t)payload_len, &ip, &icmp) != 0) {
//         return (1);
//     }
//     if (icmp->icmp_type != ICMP_TIME_EXCEEDED) {
//         return (1);
//     }
//     if (is_icmp_for_traceroute(icmp, host_addr) == false) {
//         return (1);
//     }
//     return (0);
// }

int
ft_traceloop(t_trace_res *trace_res) {}