/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:52:56 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/27 12:04:31 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICMP_H
#define ICMP_H

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>

/* IP Header Len in bytes */
#define B_IPHLEN(ip) ((ip)->ip_hl << 2U)

int      icmp_packet_decode(const uint8_t *buffer, size_t buffer_size, struct ip **p_ip, struct icmp **p_icmp);
int      icmp_packet_decode_err_udp(size_t buff_size, const struct ip *ip, const struct icmp *icmp, struct ip **org_ip,
                                    struct udphdr **org_udphdr);
uint16_t icmp_checksum(void *payload, size_t payload_len);

#endif