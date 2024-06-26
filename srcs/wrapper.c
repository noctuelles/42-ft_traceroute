/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:37:44 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/26 15:26:32 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stddef.h>
#include <sys/select.h>

#include "libft.h"

const char *
in_sock_ntop(const struct sockaddr_in *sa_in) {
    static char str[INET_ADDRSTRLEN];

    if (inet_ntop(AF_INET, &sa_in->sin_addr, str, sizeof(str)) == NULL) {
        ft_error(0, errno, "inet_ntop");
        return (NULL);
    }

    return (str);
}

struct addrinfo *
res_host_serv(const char *host, const char *serv, int sock_family, int sock_type) {
    int              ret   = 0;
    struct addrinfo  hints = {0};
    struct addrinfo *res   = NULL;

    hints.ai_flags    = AI_CANONNAME;
    hints.ai_family   = sock_family;
    hints.ai_socktype = sock_type;
    if ((ret = getaddrinfo(host, serv, &hints, &res)) != 0) {
        ft_error(0, 0, "getaddrinfo: %s,%s: %s", (host == NULL) ? "(no hostname)" : host, (serv == NULL) ? "(no service name)" : serv,
                 gai_strerror(ret));
        return (NULL);
    }
    return (res);
}

int
Socket(int domain, int type, int protocol) {
    int ret = 0;

    if ((ret = socket(domain, type, protocol)) == -1) {
        ft_error(0, errno, "socket");
    }
    return (ret);
}

void *
Calloc(size_t nmemb, size_t size) {
    void *ptr = NULL;

    if ((ptr = ft_calloc(nmemb, size)) == NULL) {
        ft_error(0, errno, "ft_calloc");
    }

    return (ptr);
}

int
Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int ret = 0;

    if ((ret = bind(sockfd, addr, addrlen)) == -1) {
        ft_error(0, errno, "bind");
    }

    return (ret);
}

ssize_t
Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
    ssize_t ret = 0;

    if ((ret = recvfrom(sockfd, buf, len, flags, src_addr, addrlen)) == -1) {
        ft_error(0, errno, "recvfrom");
    }

    return (ret);
}

int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
    int ret = 0;

    if ((ret = select(nfds, readfds, writefds, exceptfds, timeout)) == -1) {
        ft_error(0, errno, "select");
    }

    return (ret);
}

int
Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
    int ret = 0;

    if ((ret = setsockopt(sockfd, level, optname, optval, optlen)) == -1) {
        ft_error(0, errno, "setsockopt");
    }

    return (ret);
}

ssize_t
Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) {
    ssize_t ret = 0;

    if ((ret = sendto(sockfd, buf, len, flags, dest_addr, addrlen)) == -1) {
        ft_error(0, errno, "sendto");
    }

    return (ret);
}