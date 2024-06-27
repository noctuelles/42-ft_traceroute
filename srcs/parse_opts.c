/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_opts.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 22:46:38 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/27 13:58:42 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

#include "ft_args_parser.h"
#include "ft_traceroute.h"
#include "libft.h"

#define INVALID_PORT_NUMBER_STR "invalid port number"
#define NBR_TRIES_STR "number of tries must be between 1 and 10"
#define NBR_WAIT_TIME_STR "ridiculous waiting time"

int
parse_port(const char *argument, t_args_parser_state *state, void *input) {
    (void)input;

    int rslt = 0;

    if (!ft_strmatch_uint(argument)) {
        goto err;
    }
    if (ft_strlen(argument) > 5) {
        goto err;
    }
    rslt = ft_atoi(argument);
    if (rslt > UINT16_MAX) {
        goto err;
    }
    g_opts.port = (uint16_t)rslt;
    return (0);
err:
    state->error_message = INVALID_PORT_NUMBER_STR;
    state->print_arg     = true;
    return (-1);
}

int
parse_tries(const char *argument, t_args_parser_state *state, void *input) {
    (void)input;

    int rslt = 0;

    if (!ft_strmatch_uint(argument)) {
        goto err;
    }
    if (ft_strlen(argument) > 2) {
        goto err;
    }
    rslt = ft_atoi(argument);
    if (!(rslt >= 1 && rslt <= 10)) {
        goto err;
    }
    g_opts.tries = (uint8_t)rslt;
    return (0);
err:
    state->error_message = NBR_TRIES_STR;
    state->print_arg     = true;
    return (-1);
}

int
parse_resolve_hostnames(const char *argument, t_args_parser_state *state, void *input) {
    (void)argument;
    (void)state;
    (void)input;

    g_opts.resolve_hostname = true;

    return (0);
}

int
parse_wait(const char *argument, t_args_parser_state *state, void *input) {
    (void)input;

    int rslt = 0;

    if (!ft_strmatch_uint(argument)) {
        goto err;
    }
    if (ft_strlen(argument) > 2) {
        goto err;
    }
    rslt = ft_atoi(argument);
    if (!(rslt >= 1 && rslt <= 60)) {
        goto err;
    }
    g_opts.wait_time = (time_t)rslt;
    return (0);
err:
    state->error_message = NBR_WAIT_TIME_STR;
    state->print_arg     = true;
    return (-1);
}

int
parse_help(const char *argument, t_args_parser_state *state, void *input) {
    (void)argument;
    (void)state;
    (void)input;

    g_opts.help = true;

    return (0);
}

int
parse_argument(const char *argument, t_args_parser_state *state, void *input) {
    (void)input;
    (void)state;

    const char **host = NULL;

    host  = input;
    *host = argument;

    return (0);
}
