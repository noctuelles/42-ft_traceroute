/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:50:32 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/26 15:17:40 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRAPPER_H
#define WRAPPER_H

const char      *in_sock_ntop(const struct sockaddr_in *sa_in);
struct addrinfo *res_host_serv(const char *host, const char *serv, int sock_family, int sock_type);
int              Socket(int domain, int type, int protocol);
void            *Calloc(size_t nmemb, size_t size);
int              Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t          Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
int              Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

#endif