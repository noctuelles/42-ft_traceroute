/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:00:28 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/26 11:12:28 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>
#include <sys/socket.h>

typedef struct s_fd_sock {
    int                fd;
    const char        *name;
    char               ipv4[INET_ADDRSTRLEN];
    struct sockaddr_in addr;
} t_fd_sock;

int resolve_host(const char *host, t_fd_sock *res_host);
int new_icmp_sock(t_fd_sock *peer);
int set_sockopts(int sock_fd);

#endif