/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 22:51:08 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/24 20:12:51 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "socket.h"

#define AUTHOR "plouvel"
#define PROBE_CONTENT "SUPERMAN"

#define DFT_FIRST_HOP 1u
#define DFT_WAIT_TIME_SEC 3u
#define DFT_PORT 33434u

typedef struct s_opts {
    uint8_t  first_hop; /* set initial hop distance (TTL) --first-hop */
    uint8_t  max_hops;  /* set maximum hop distance. -m --max-hop */
    uint8_t  tries;     /* set number of tries per hop. -q --tries */
    uint16_t port;      /* destination port. -p --port */
    time_t   wait_time; /* set time to wait for a response in seconds. -w --wait */
    bool     icmp;      /* use ICMP ECHO requests instead of UDP datagrams. -I */
    bool     help;      /* display help message. -h --help */
} t_opts;

extern t_opts g_opts;

int traceroute(const t_fd_sock *res_host, t_fd_sock *peer);

#endif