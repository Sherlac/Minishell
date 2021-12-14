/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmariot <cmariot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 15:17:49 by cmariot           #+#    #+#             */
/*   Updated: 2021/12/14 17:26:28 by cmariot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_minishell(t_command_line *command_line)
{
	if (command_line->line != NULL)
		free(command_line->line);
	command_line->line = NULL;
	ft_free_array(command_line->splitted_line);
	if (command_line->main.command != NULL)
		free(command_line->main.command);
	command_line->main.command = NULL;
	if (command_line->main.args != NULL)
		ft_free_array(command_line->main.args);
	command_line->main.args = NULL;
}
