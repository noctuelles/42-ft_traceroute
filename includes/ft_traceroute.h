/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 22:51:08 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/26 16:09:52 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include <time.h>

#include "socket.h"

#define AUTHOR "plouvel"
#define PROBE_CONTENT "SUPERMAN"

#define DFT_FIRST_HOP 1u
#define DFT_MAX_HOPS 64u
#define DFT_TRIES 3u
#define DFT_WAIT_TIME_SEC 3u
#define DFT_PORT 33434u

typedef struct s_opts {
    uint8_t  first_hop; /* set initial hop distance (TTL) --first-hop */
    uint8_t  max_hops;  /* set maximum hop distance. -m --max-hop */
    uint8_t  tries;     /* set number of tries per hop. -q --tries */
    uint16_t port;      /* destination port. -p --port */
    time_t   wait_time; /* set time to wait for a response in seconds. -w --wait */
    bool     help;      /* display help message. -h --help */
    bool     resolve_hostname;
} t_opts;

typedef struct s_trace_res {
    int                 fd_recv;
    int                 fd_send;
    struct sockaddr_in *sa_recv;
    struct sockaddr_in *sa_send;
    struct sockaddr_in *sa_bind;
    struct sockaddr_in *sa_last;
    const socklen_t     sa_len;
} t_trace_res;

typedef enum e_res_code {
    RES_CODE_INTERAL_ERR = -1,
    RES_CODE_DONE,
    RES_CODE_TIMEOUT,
    RES_CODE_KEEP_GOING,
} t_res_code;

extern t_opts g_opts;

int ft_traceloop(t_trace_res *trace_res);

#endif