/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:51:04 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/27 14:14:16 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "icmp.h"

#include <stddef.h>
#include <stdio.h>

/**
 * @brief compute the internet checksum of a payload for an ICMPv4 packet (ICMPv6 is computed by the kernel)
 * @note see https://tools.ietf.org/html/rfc1071
 *
 * @param payload pointer to the payload
 * @param payload_len size of the payload
 * @return uint16_t the checksum
 */
uint16_t
icmp_checksum(void *payload, size_t payload_len) {
    uint32_t  checksum   = 0;
    uint16_t *ptr        = (uint16_t *)payload;
    int32_t   bytes_left = (int32_t)payload_len;

    while (bytes_left > 1) {
        checksum += *ptr++;
        checksum = (checksum & 0xffff) + (checksum >> 16);
        bytes_left -= 2;
    }

    if (bytes_left == 1) {
        checksum += *(uint8_t *)ptr;
        checksum = (checksum & 0xffff) + (checksum >> 16);
    }

    return (uint16_t)(~checksum & 0xffff);
}

/**
 * @brief Decode an incoming raw buffer, and put the IP and ICMP header in the pointers provided.
 *
 * @param buffer pointer to the buffer
 * @param buffer_size size of the buffer
 * @param p_ip pointer to the pointer that will receive the IP header
 * @param p_icmp pointer to the pointer that will receive the ICMP header
 * @return int 0 if the packet is valid, 1 if the checksum is invalid, -1 if the buffer is too small.
 */
int
icmp_packet_decode(const uint8_t *buff, size_t buff_size, struct ip **p_ip, struct icmp **p_icmp) {
    uint16_t     icmplen = 0;
    uint16_t     chksum  = 0;
    struct ip   *ip      = NULL;
    struct icmp *icmp    = NULL;

    ip      = (struct ip *)(buff);
    icmplen = buff_size - B_IPHLEN(ip);
    if (icmplen < ICMP_MINLEN) {
        /* Not enough data to read ICMP. */
        return (-1);
    }
    icmp             = (struct icmp *)(buff + B_IPHLEN(ip));
    chksum           = icmp->icmp_cksum;
    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = icmp_checksum(icmp, icmplen);
    if (chksum != icmp->icmp_cksum) {
        /* Checksum mismatch. */
        return (-2);
    }
    *p_ip   = ip;
    *p_icmp = icmp;
    return (0);
}

/**
 * @brief Decode an ICMP error message, and put the original IP and UDP headers in the pointers provided.
 *
 * @param buff_size size of the buffer returned by recvfrom.
 * @param ip decoded IP structure.
 * @param icmp decoded ICMP structure.
 * @param org_ip result-argument for the failed (original) IP packet.
 * @param org_udphdr result-argument for the failed (original) UDP packet.
 * @return int 0 if the packet is valid, -1 a problem occurred.
 */
int
icmp_packet_decode_err_udp(size_t buff_size, const struct ip *ip, const struct icmp *icmp, struct ip **org_ip, struct udphdr **org_udphdr) {
    uint16_t       icmplen      = 0;
    struct ip     *p_org_ip     = NULL;
    struct udphdr *p_org_udphdr = NULL;

    icmplen = buff_size - B_IPHLEN(ip);
    if (icmplen < ICMP_MINLEN + sizeof(struct ip)) {
        /* Not enough data to read IP header (without options) from ICMP data. */
        return (-1);
    }
    p_org_ip = (struct ip *)icmp->icmp_data;
    if (p_org_ip->ip_p != IPPROTO_UDP) {
        /* ICMP Error Message is not the result of an UDP packet. */
        puts("not udp");
        return (-1);
    }
    if (icmplen < ICMP_MINLEN + B_IPHLEN(p_org_ip) + sizeof(struct udphdr)) {
        /* Not enough data to read the UDP header (without data) from ICMP data. */
        return (-1);
    }
    p_org_udphdr = (struct udphdr *)(((uint8_t *)p_org_ip) + B_IPHLEN(p_org_ip));
    *org_ip      = p_org_ip;
    *org_udphdr  = p_org_udphdr;
    return (0);
}