/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:00:28 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/24 14:01:19 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>

typedef struct s_resolved_host {
    int             fd;
    struct sockaddr addr;
    socklen_t       addrlen;
} t_resolved_host;

int resolve_host(const char *host, t_resolved_host *res_host);

#endif