/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_opts.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 22:46:49 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/27 14:24:58 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_OPTS_H
#define PARSE_OPTS_H

#include "ft_args_parser.h"

int parse_port(const char *argument, t_args_parser_state *state, void *input);
int parse_tries(const char *argument, t_args_parser_state *state, void *input);
int parse_wait(const char *argument, t_args_parser_state *state, void *input);
int parse_resolve_hostnames(const char *argument, t_args_parser_state *state, void *input);
int parse_first_hop(const char *argument, t_args_parser_state *state, void *input);
int parse_max_hop(const char *argument, t_args_parser_state *state, void *input);

int parse_argument(const char *argument, t_args_parser_state *state, void *input);
int parse_help(const char *argument, t_args_parser_state *state, void *input);

#endif