/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 22:43:13 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/24 13:42:26 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "ft_args_parser.h"
#include "ft_traceroute.h"
#include "parse_opts.h"

extern const char *program_invocation_short_name;

static t_args_parser_option_entry opt_entries[] = {
    {.has_argument                  = true,
     .parse_fn                      = parse_port,
     .short_key                     = "p",
     .long_key                      = "port",
     .long_key_argument_description = "PORT",
     .description                   = "use destination PORT port (default 33434)"},

    {.has_argument                  = false,
     .parse_fn                      = parse_help,
     .short_key                     = "h",
     .long_key                      = "help",
     .long_key_argument_description = NULL,
     .description                   = "display this help and exit"},
};

t_opts g_opts = {0};

int
print_help(const t_args_parser_config *config) {
    printf("Usage: %s [OPTION]... HOST\n", program_invocation_short_name);
    printf("Print the route packets trace to network host.\n\n");
    ft_args_parser_print_docs(config);
    printf("\nWritten by %s.\n", AUTHOR);
    return (0);
}

int
main(int argc, char **argv) {
    char                *host   = NULL;
    t_args_parser_config config = {.argc                      = argc,
                                   .argv                      = argv,
                                   .entries                   = opt_entries,
                                   .entries_nbr               = sizeof(opt_entries) / sizeof(t_args_parser_option_entry),
                                   .input                     = &host,
                                   .default_argument_parse_fn = parse_argument};

    if (ft_args_parser(&config) == -1) {
        return (1);
    }
    if (g_opts.help) {
        return (print_help(&config));
    }
    return (0);
}
