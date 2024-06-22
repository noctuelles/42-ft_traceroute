/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 22:43:13 by plouvel           #+#    #+#             */
/*   Updated: 2024/06/22 22:45:25 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_args_parser.h"

static t_args_parser_option_entry opt_entries[] = {};

int
main(int argc, char **argv) {
    t_args_parser_config config = {
        .argc = argc, .argv = argv, .entries = opt_entries, .entries_nbr = sizeof(opt_entries) / sizeof(t_args_parser_option_entry)};

    if (ft_args_parser(&config) == -1) {
        return (1);
    }

    return (0);
}