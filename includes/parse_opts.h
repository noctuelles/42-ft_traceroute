/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_opts.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 22:46:49 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/22 23:33:44 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_OPTS_H
#define PARSE_OPTS_H

#include "ft_args_parser.h"

int parse_port(const char *argument, t_args_parser_state *state, void *input);
int parse_argument(const char *argument, t_args_parser_state *state, void *input);
int parse_help(const char *argument, t_args_parser_state *state, void *input);

#endif