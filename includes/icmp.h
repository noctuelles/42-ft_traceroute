/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:52:56 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/25 17:53:38 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICMP_H
#define ICMP_H

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

int      icmp_packet_decode(const uint8_t *buffer, size_t buffer_size, struct ip **p_ip, struct icmp **p_icmp);
uint16_t icmp_checksum(void *payload, size_t payload_len);

#endif