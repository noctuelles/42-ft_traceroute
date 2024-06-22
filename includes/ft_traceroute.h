/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 22:51:08 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/22 23:36:31 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <stdint.h>

#define AUTHOR "plouvel"

typedef struct s_opts {
    uint8_t  first_hop; /* set initial hop distance (TTL) --first-hop */
    uint8_t  max_hops;  /* set maximum hop distance. -m --max-hop */
    uint8_t  tries;     /* set number of tries per hop. -q --tries */
    uint16_t port;      /* destination port. -p --port */
    bool     icmp;      /* use ICMP ECHO requests instead of UDP datagrams. -I */
    bool     help;      /* display help message. -h --help */
} t_opts;

extern t_opts g_opts;

#endif