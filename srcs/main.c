/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 22:43:13 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/27 18:59:53 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ft_args_parser.h"
#include "ft_traceroute.h"
#include "libft.h"
#include "parse_opts.h"
#include "wrapper.h"

/**
 * @brief Program options
 *
 */
t_opts g_opts = {
    .port             = DFT_PORT,
    .first_hop        = DFT_FIRST_HOP,
    .max_hops         = DFT_MAX_HOPS,
    .wait_time        = DFT_WAIT_TIME_SEC,
    .tries            = DFT_TRIES,
    .help             = false,
    .resolve_hostname = false,
};

extern const char *program_invocation_short_name;

static t_args_parser_option_entry opt_entries[] = {
    {.has_argument                  = true,
     .parse_fn                      = parse_port,
     .short_key                     = "p",
     .long_key                      = "port",
     .long_key_argument_description = "PORT",
     .description                   = "use destination PORT port (default 33434)"},

    {.has_argument                  = true,
     .parse_fn                      = parse_tries,
     .short_key                     = "q",
     .long_key                      = "tries",
     .long_key_argument_description = "NUM",
     .description                   = "send NUM probe packets per hop (default: 3)"},

    {.has_argument                  = true,
     .parse_fn                      = parse_wait,
     .short_key                     = "w",
     .long_key                      = "wait",
     .long_key_argument_description = "NUM",
     .description                   = "wait NUM seconds for response (default: 3)"},

    {.has_argument                  = true,
     .parse_fn                      = parse_first_hop,
     .short_key                     = "f",
     .long_key                      = "first-hop",
     .long_key_argument_description = "NUM",
     .description                   = "set initial hop distance, i.e., time-to-live"},

    {.has_argument                  = true,
     .parse_fn                      = parse_max_hop,
     .short_key                     = "m",
     .long_key                      = "max-hop",
     .long_key_argument_description = "NUM",
     .description                   = "set maximal hop count (default: 64)"},

    {.has_argument                  = false,
     .parse_fn                      = parse_resolve_hostnames,
     .short_key                     = NULL,
     .long_key                      = "resolve-hostnames",
     .long_key_argument_description = NULL,
     .description                   = "resolve hostnames"},

    {.has_argument                  = false,
     .parse_fn                      = parse_help,
     .short_key                     = "h",
     .long_key                      = "help",
     .long_key_argument_description = NULL,
     .description                   = "display this help and exit"},
};

static int
print_help(const t_args_parser_config *config) {
    printf("Usage: %s [OPTION]... HOST\n", program_invocation_short_name);
    printf("Print the route packets trace to network host (only for ipv4).\n\n");
    ft_args_parser_print_docs(config);
    printf("\nWritten by %s.\n", AUTHOR);
    return (0);
}

int
main(int argc, char **argv) {
    const char          *host   = NULL;
    t_args_parser_config config = {.argc                      = argc,
                                   .argv                      = argv,
                                   .entries                   = opt_entries,
                                   .entries_nbr               = sizeof(opt_entries) / sizeof(t_args_parser_option_entry),
                                   .input                     = &host,
                                   .default_argument_parse_fn = parse_argument};
    struct addrinfo     *res    = NULL;
    int                  ret    = 1;
    t_trace_res          trace  = {.sa_len = sizeof(struct sockaddr_in)};

    if (ft_args_parser(&config) == -1) {
        return (1);
    }
    if (g_opts.help) {
        return (print_help(&config));
    }
    if (host == NULL) {
        ft_error(0, 0, "missing HOST argument");
        return (1);
    }
    if ((res = res_host_serv(host, NULL, AF_INET, SOCK_DGRAM)) == NULL) {
        goto end;
    }
    if ((trace.fd_send = Socket(res->ai_family, res->ai_socktype, 0)) == -1) {
        goto clean_res;
    }
    if ((trace.fd_recv = Socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
        goto clean_fd_send;
    }
    trace.sa_send = (struct sockaddr_in *)res->ai_addr;
    if ((trace.sa_recv = Calloc(1, trace.sa_len)) == NULL) {
        goto clean_fd_recv;
    }
    if ((trace.sa_bind = Calloc(1, trace.sa_len)) == NULL) {
        goto clean_sa;
    }
    if ((trace.sa_last = Calloc(1, trace.sa_len)) == NULL) {
        goto clean_sa;
    }
    trace.sa_bind->sin_family = trace.sa_send->sin_family;
    trace.sa_bind->sin_port =
        htons((getpid() & 0xffff) | (1U << 15)); /* Source port is the PID of the current process, with the high-order bit set to one. */
    if (Bind(trace.fd_send, (const struct sockaddr *)trace.sa_bind, trace.sa_len) == -1) {
        goto clean_sa;
    }
    if ((host = in_sock_ntop(trace.sa_send)) == NULL) {
        goto clean_sa;
    }
    printf("traceroute to %s (%s), %u hops max\n", res->ai_canonname != NULL ? res->ai_canonname : host, host, g_opts.max_hops);
    if (ft_traceloop(&trace) != 0) {
        goto clean_sa;
    }
    ret = 0;
clean_sa:
    free(trace.sa_recv);
    free(trace.sa_bind);
    free(trace.sa_last);
clean_fd_recv:
    close(trace.fd_recv);
clean_fd_send:
    close(trace.fd_send);
clean_res:
    freeaddrinfo(res);
end:
    return (ret);
}
