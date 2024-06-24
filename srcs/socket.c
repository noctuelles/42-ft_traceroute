/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:42:20 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/24 14:04:39 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _DEFAULT_SOURCE
#include "socket.h"

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "libft.h"

/**
 * @brief Resolve a hostname to a socket file descriptor and a sockaddr structure.
 *
 * @param host The hostname to resolve.
 * @param res_host The structure to store the resolved host.
 * @return int -1 if an error occured, 0 otherwise.
 */
int
resolve_host(const char *host, t_resolved_host *res_host) {
    struct addrinfo    hints         = {0};
    struct addrinfo   *res           = NULL;
    struct addrinfo   *elem_res      = NULL;
    struct sockaddr_in hint_sockaddr = {0};
    int                ret           = 0;

    hints.ai_family   = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_socktype = SOCK_DGRAM;

    if ((ret = getaddrinfo(host, NULL, &hints, &res)) != 0) {
        ft_error(0, 0, "getaddrinfo: %s\n", gai_strerror(ret));
        return (-1);
    }
    elem_res = res;
    while (elem_res != NULL) {
        ret = socket(elem_res->ai_family, elem_res->ai_socktype, elem_res->ai_protocol);
        if (ret != -1) {
            res_host->fd      = ret;
            res_host->addr    = *elem_res->ai_addr;
            res_host->addrlen = elem_res->ai_addrlen;
            break;
        }
        elem_res = elem_res->ai_next;
    }
    if (ret == -1) {
        ft_error(0, 0, "socket: %s\n", strerror(errno));
        return (-1);
    }
    freeaddrinfo(res);
    return (0);
}