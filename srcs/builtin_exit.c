/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmariot <cmariot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 08:46:08 by cmariot           #+#    #+#             */
/*   Updated: 2022/01/14 16:19:47 by cmariot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error_not_num(char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	change_global_exit_status(2);
}

void	exit_error(char *first_arg, char**args)
{
	if (*first_arg != '\0')
	{
		exit_error_not_num(args[0]);
	}
	else
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		change_global_exit_status(1);
	}
}

void	builtin_exit(t_shell *minishell, char **args)
{
	long long	exit_status;
	char		*first_arg;

	if (args[0] == NULL)
		change_global_exit_status(0);
	else
	{
		first_arg = args[0];
		exit_status = ft_strtoll(first_arg, &first_arg);
		if (args[1] == NULL)
		{
			if (*first_arg != '\0')
				exit_error_not_num(args[0]);
			else
			{
				if (exit_status > 255)
					exit_status -= 256;
				change_global_exit_status(exit_status);
			}
		}
		else
			exit_error(first_arg, args);
	}
	free_minishell(minishell);
}
