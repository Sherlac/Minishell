/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/29 13:48:19 by flee              #+#    #+#             */
/*   Updated: 2021/12/31 10:34:51 by cmariot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*new_path(t_shell *minishell, char *path)
{
	char	*home;
	char	*new_path;
	size_t	size;

	size = ft_strlen(path) - 1;
	home = get_env_value("HOME", minishell->env);
	path = ft_substr((const char *)path, 1, size);
	new_path = ft_strjoin(home, path);
	free(path);
	return (new_path);
}

void	do_cd(t_shell *minishell)
{
	char		*path;
	int			ret;
	char		*cwd;

	//pourquoi cette condition ?
	//if (minishell->command_line.number_of_simple_commands != 2)
	//{
		path = minishell->command_line.command->command_and_args[1];
		if (path)
		{
			if (path[0] == '~')
				path = new_path(minishell, path);
			ret = chdir(path);
		}
		else
			ret = chdir(get_env_value("HOME", minishell->env));
		if (ret != 0)
			printf("minishell: cd: %s: No such file or directory\n",
				minishell->command_line.command->command_and_args[1]);
	//}
	cwd = getcwd(NULL, 1);
	setenv_builtin(minishell->env, "PWD", cwd);
	free(cwd);
}
