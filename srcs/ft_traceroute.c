/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:31:09 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/24 20:14:25 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

#include <arpa/inet.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "libft.h"
#include "socket.h"

int
traceroute(const t_fd_sock *res_host, t_fd_sock *peer) {
    fd_set         readfds;
    struct timeval timeout = {0};
    int            ret     = 0;

    FD_ZERO(&readfds);
    FD_SET(peer->fd, &readfds);
    timeout.tv_sec  = g_opts.wait_time != 0 ? g_opts.wait_time : DFT_WAIT_TIME_SEC;
    timeout.tv_usec = 0;
    while (true) {
        if (sendto(res_host->fd, PROBE_CONTENT, sizeof(PROBE_CONTENT), 0, &res_host->addr, res_host->addrlen) == -1) {
            ft_error(0, 0, "sendto: %s", strerror(errno));
            return (-1);
        }
        ret = select(peer->fd + 1, &readfds, NULL, NULL, &timeout);
        if (ret == -1) {
            ft_error(0, 0, "select: %s", strerror(errno));
            return (-1);
        } else if (ret == 0) {
            puts("Timeout");
        } else {
            puts("Message recv !");
        }
        FD_SET(peer->fd, &readfds);
        timeout.tv_sec  = g_opts.wait_time != 0 ? g_opts.wait_time : DFT_WAIT_TIME_SEC;
        timeout.tv_usec = 0;
    }
    return (ret);
}