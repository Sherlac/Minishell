/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmariot <cmariot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:08:46 by cmariot           #+#    #+#             */
/*   Updated: 2021/12/27 23:28:55 by cmariot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// In the env linked list, get a copy of the value
// of the elememt which have name = "name",
// if not found, return NULL. */
char	*get_env_value(char *name, t_env *env)
{
	char	*value;

	value = NULL;
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
		{
			value = ft_strdup(env->value);
			break ;
		}
		env = env->next;
	}
	return (value);
}

int	current_directory_len(char *pwd)
{
	int	pwd_last_index;
	int	cd_len;

	pwd_last_index = ft_strlen(pwd) - 1;
	cd_len = 0;
	while (pwd[pwd_last_index] != '/' && pwd_last_index >= 0)
	{
		cd_len++;
		pwd_last_index--;
	}
	return (cd_len);
}

// The prompts is set as the name of the current directory.
char	*get_prompt(t_shell *minishell)
{
	char	*pwd;
	int		pwd_last_index;
	char	*current_directory;
	int		cur_dir_len;
	char	*prompt;

	pwd = get_env_value("PWD", minishell->env);
	if (pwd == NULL)
		return (ft_strdup("Minishell ➤ "));
	cur_dir_len = current_directory_len(pwd);
	current_directory = ft_calloc(cur_dir_len + 1, sizeof(char));
	if (!current_directory)
		return (ft_strdup("Minishell ➤ "));
	pwd_last_index = ft_strlen(pwd) - 1;
	while (cur_dir_len - 1 >= 0)
	{
		current_directory[cur_dir_len-- - 1] = pwd[pwd_last_index--];
	}
	prompt = ft_strjoin(current_directory, " ➤ ");
	free(current_directory);
	free(pwd);
	return (prompt);
}

// Get the prompt and use readline to show the prompt and store the input line
void	get_command_line(t_shell *minishell, t_command_line *command_line)
{
	char	*prompt;

	prompt = get_prompt(minishell);
	if (command_line->line != NULL)
		free(command_line->line);
	command_line->line = readline(prompt);
	if (rl_on_new_line() == 0)
		add_history(command_line->line);
	free(prompt);
	if (minishell->command_line.line == NULL)
	{
		printf("exit\n");
		free_minishell(minishell);
		exit(EXIT_SUCCESS);
	}
}
