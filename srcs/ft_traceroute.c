/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:31:09 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/26 16:13:39 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

#include <netdb.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <sys/socket.h>

#include "libft.h"
#include "wrapper.h"

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

static t_res_code
recv_packet(t_trace_res *trace_res) {
    uint8_t   payload[IP_MAXPACKET] = {0};
    socklen_t sa_len                = trace_res->sa_len;

    if (Recvfrom(trace_res->fd_recv, payload, sizeof(payload), 0, (struct sockaddr *)trace_res->sa_recv, &sa_len) == -1) {
        return (RES_CODE_INTERAL_ERR);
    }
    (void)sa_len;

    return (RES_CODE_KEEP_GOING);
}

static t_res_code
await_response(t_trace_res *trace_res) {
    fd_set         readfds;
    struct timeval timeout;
    t_res_code     ret = 0;

    FD_ZERO(&readfds);
    FD_SET(trace_res->fd_recv, &readfds);
    timeout.tv_sec  = g_opts.wait_time != 0 ? g_opts.wait_time : DFT_WAIT_TIME_SEC;
    timeout.tv_usec = 0;

    if ((ret = Select(trace_res->fd_recv + 1, &readfds, NULL, NULL, &timeout)) == -1) {
        return (RES_CODE_INTERAL_ERR);
    } else if (ret == 0) {
        return (RES_CODE_TIMEOUT);
    }
    return (recv_packet(trace_res));
}

int
ft_traceloop(t_trace_res *trace_res) {
    t_res_code ret              = 0;
    char       host[NI_MAXHOST] = {0};

    for (int nhops = g_opts.first_hop; nhops < g_opts.max_hops; nhops++) {
        if (Setsockopt(trace_res->fd_send, IPPROTO_IP, IP_TTL, &nhops, sizeof(nhops)) == -1) {
            return (-1);
        }
        ft_bzero(trace_res->sa_last, trace_res->sa_len);
        printf("%2d ", nhops);
        fflush(stdout);
        for (uint32_t nprobe = 0; nprobe < g_opts.tries; nprobe++) {
            trace_res->sa_send->sin_port = htons(g_opts.port);
            if (Sendto(trace_res->fd_send, PROBE_CONTENT, sizeof(PROBE_CONTENT), 0, (const struct sockaddr *)trace_res->sa_send,
                       trace_res->sa_len) == -1) {
                return (-1);
            }
            ret = await_response(trace_res);
            if (ret == RES_CODE_INTERAL_ERR) {
                return (-1);
            } else if (ret == RES_CODE_TIMEOUT) {
                printf(" *");
            } else {
                if (ft_memcmp(trace_res->sa_last, trace_res->sa_recv, trace_res->sa_len) != 0) {
                    printf(" %s", in_sock_ntop(trace_res->sa_recv));
                    if (g_opts.resolve_hostname &&
                        getnameinfo((const struct sockaddr *)trace_res->sa_recv, trace_res->sa_len, host, sizeof(host), NULL, 0, 0) == 0) {
                        printf(" (%s)", host);
                    }
                    ft_memcpy(trace_res->sa_last, trace_res->sa_recv, trace_res->sa_len);
                }
            }
            fflush(stdout);
        }
        printf("\n");
    }
    return (0);
}