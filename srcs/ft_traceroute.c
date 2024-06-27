/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:31:09 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/27 18:56:45 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

#include <netdb.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "icmp.h"
#include "libft.h"
#include "wrapper.h"

static bool
is_icmp_err_from_our_curr_probe(const t_trace_res *trace_res, const struct ip *org_ip, const struct udphdr *org_udphdr) {
    if (org_ip->ip_dst.s_addr != trace_res->sa_send->sin_addr.s_addr) {
        return (false);
    }
    if (org_udphdr->uh_sport != trace_res->sa_bind->sin_port) {
        return (false);
    }
    if (org_udphdr->uh_dport != trace_res->sa_send->sin_port) {
        return (false);
    }
    return (true);
}

static t_res_code
recv_packet(t_trace_res *trace_res) {
    static uint8_t buffer[IP_MAXPACKET] = {0};
    ssize_t        buff_size            = 0;
    socklen_t      sa_len               = trace_res->sa_len;
    struct ip     *ip                   = NULL;
    struct icmp   *icmp                 = NULL;
    struct ip     *org_ip               = NULL;
    struct udphdr *org_udphdr           = NULL;

    if ((buff_size = Recvfrom(trace_res->fd_recv, buffer, sizeof(buffer), 0, (struct sockaddr *)trace_res->sa_recv, &sa_len)) == -1) {
        return (RES_CODE_INTERAL_ERR);
    }
    if (icmp_packet_decode(buffer, (size_t)buff_size, &ip, &icmp) != 0) {
        return (RES_CODE_AWAIT);
    }
    if (icmp->icmp_type == ICMP_TIME_EXCEEDED && icmp->icmp_code == ICMP_EXC_TTL) {
        if (icmp_packet_decode_err_udp((size_t)buff_size, ip, icmp, &org_ip, &org_udphdr) != 0) {
            return (RES_CODE_AWAIT);
        }
        if (is_icmp_err_from_our_curr_probe(trace_res, org_ip, org_udphdr) == false) {
            return (RES_CODE_AWAIT);
        }
        return (RES_CODE_TIME_EXCEEDED);
    } else if (icmp->icmp_type == ICMP_DEST_UNREACH && icmp->icmp_code == ICMP_PORT_UNREACH) {
        if (icmp_packet_decode_err_udp((size_t)buff_size, ip, icmp, &org_ip, &org_udphdr) != 0) {
            return (RES_CODE_AWAIT);
        }
        if (is_icmp_err_from_our_curr_probe(trace_res, org_ip, org_udphdr) == false) {
            return (RES_CODE_AWAIT);
        }
        return (RES_CODE_DEST_UNREACH);
    }
    return (RES_CODE_AWAIT);
}

/**
 * @brief During 3 seconds (default value), wait for a response from the remote host. An ICMP Error Message which is not targeted to our
 * current probe is ignored (see recv_packet), and select is rearmed.
 *
 * @param trace_res
 * @return t_res_code RES_CODE_TIMEOUT, RES_CODE_DEST_UNREACH, RES_CODE_TIME_EXCEEDED, RES_CODE_INTERNAL_ERR
 *
 * @note From 2020-11-01 SELECT man page : "On  Linux, select() modifies timeout to reflect the amount of time not slept; most other
 * implementations do not do this."
 */
static t_res_code
await_response(t_trace_res *trace_res) {
    fd_set         readfds;
    struct timeval timeout;
    t_res_code     ret = RES_CODE_AWAIT;

    FD_ZERO(&readfds);
    FD_SET(trace_res->fd_recv, &readfds);
    timeout.tv_sec  = g_opts.wait_time != 0 ? g_opts.wait_time : DFT_WAIT_TIME_SEC;
    timeout.tv_usec = 0;
    while (true) {
        if ((ret = Select(trace_res->fd_recv + 1, &readfds, NULL, NULL, &timeout)) == -1) {
            return (RES_CODE_INTERAL_ERR);
        } else if (ret == 0) {
            return (RES_CODE_TIMEOUT);
        } else {
            ret = recv_packet(trace_res);
            if (ret != RES_CODE_AWAIT) {
                return (ret);
            }
        }
    }
    return (RES_CODE_INTERAL_ERR);
}

static double
get_tv_diff_ms(const struct timeval *tv1, const struct timeval *tv2) {
    return ((tv1->tv_sec - tv2->tv_sec) * 1e3 + (tv1->tv_usec - tv2->tv_usec) / 1e3);
}

/**
 * @brief Main loop of the traceroute program.
 *
 * @param trace_res trace resources.
 * @return int 0 on success, -1 on error.
 */
int
ft_traceloop(t_trace_res *trace_res) {
    t_res_code     ret              = 0;
    char           host[NI_MAXHOST] = {0};
    uint32_t       done             = 0;
    struct timeval sent_tv = {0}, recv_tv = {0};

    for (int nhops = g_opts.first_hop; nhops < g_opts.first_hop + g_opts.max_hops && done == 0; nhops++) {
        if (Setsockopt(trace_res->fd_send, IPPROTO_IP, IP_TTL, &nhops, sizeof(nhops)) == -1) {
            return (-1);
        }
        ft_bzero(trace_res->sa_last, trace_res->sa_len);
        printf("%2d ", nhops - g_opts.first_hop + 1);
        fflush(stdout);
        for (uint32_t nprobe = 0; nprobe < g_opts.tries; nprobe++) {
            (void)gettimeofday(&sent_tv, NULL);
            trace_res->sa_send->sin_port = htons(g_opts.port + nhops);
            if (Sendto(trace_res->fd_send, PROBE_CONTENT, sizeof(PROBE_CONTENT), 0, (const struct sockaddr *)trace_res->sa_send,
                       trace_res->sa_len) == -1) {
                return (-1);
            }
            ret = await_response(trace_res);
            if (ret == RES_CODE_INTERAL_ERR) {
                return (-1);
            } else if (ret == RES_CODE_TIMEOUT) {
                printf("  *");
            } else if (ret == RES_CODE_DEST_UNREACH || ret == RES_CODE_TIME_EXCEEDED) {
                (void)gettimeofday(&recv_tv, NULL);
                if (ft_memcmp(trace_res->sa_last, trace_res->sa_recv, trace_res->sa_len) != 0) {
                    printf("  %s", in_sock_ntop(trace_res->sa_recv));
                    if (g_opts.resolve_hostname &&
                        getnameinfo((const struct sockaddr *)trace_res->sa_recv, trace_res->sa_len, host, sizeof(host), NULL, 0, 0) == 0) {
                        printf(" (%s)", host);
                    }
                    ft_memcpy(trace_res->sa_last, trace_res->sa_recv, trace_res->sa_len);
                }
                printf("  %.3fms", get_tv_diff_ms(&recv_tv, &sent_tv));
                if (ret == RES_CODE_DEST_UNREACH) {
                    done = done + 1;
                }
            }
            fflush(stdout);
        }
        printf("\n");
    }
    return (done == 0 ? -1 : 0);
}