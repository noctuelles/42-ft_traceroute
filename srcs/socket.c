/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:42:20 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/26 11:15:25 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _DEFAULT_SOURCE

#include "socket.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "ft_traceroute.h"
#include "libft.h"

/**
 * @brief Resolve a hostname to a socket file descriptor and a sockaddr structure.
 *
 * @param host The hostname to resolve.
 * @param res_host The structure to store the resolved host.
 * @return int -1 if an error occured, 0 otherwise.
 */
int
resolve_host(const char *host, t_fd_sock *res_host) {
    struct addrinfo  hints    = {0};
    struct addrinfo *res      = NULL;
    struct addrinfo *elem_res = NULL;
    int              ret      = 0;

    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if ((ret = getaddrinfo(host, NULL, &hints, &res)) != 0) {
        ft_error(0, 0, "getaddrinfo: %s", gai_strerror(ret));
        return (-1);
    }
    elem_res = res;
    while (elem_res != NULL) {
        ret = socket(elem_res->ai_family, elem_res->ai_socktype, 0);
        if (ret != -1) {
            res_host->fd   = ret;
            res_host->addr = *(struct sockaddr_in *)elem_res->ai_addr;
            res_host->name = host;
            (void)inet_ntop(AF_INET, &res_host->addr.sin_addr, res_host->ipv4, sizeof(res_host->ipv4));
            ret = 0;
            break;
        }
        elem_res = elem_res->ai_next;
    }
    if (ret == -1) {
        ft_error(0, 0, "socket: %s", strerror(errno));
    }
    freeaddrinfo(res);
    return (ret);
}

int
new_icmp_sock(t_fd_sock *peer) {
    if ((peer->fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
        ft_error(0, 0, "socket: %s", strerror(errno));
        return (-1);
    }
    ft_bzero(&peer->addr, sizeof(peer->addr));
    return (0);
}