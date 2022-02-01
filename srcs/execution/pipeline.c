/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmariot <cmariot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/30 13:56:14 by cmariot           #+#    #+#             */
/*   Updated: 2022/01/31 14:58:44 by cmariot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fork_error(void)
{
	print(2, "minishell: error: fork failed.\n");
	return (1);
}

int	pipe_error(void)
{
	print(2, "minishell: error: pipe failed.\n");
	return (1);
}

/* For the last pipe, STDIN is already redirect, so just fork.
 * In the child, execute command and exit.
 * In the parent, restore the file_descriptor */
int	last_pipe(t_shell *minishell, t_simple command, int *backup_fd)
{
	int	status;

	if (pipe(command.pipe_fd) == -1)
		return (pipe_error());
	command.pid = fork();
	if (command.pid == -1)
		return (fork_error());
	else if (command.pid == 0)
	{
		execute_simple_command(minishell, command, backup_fd);
		status = return_global_exit_status();
		close(command.pipe_fd[0]);
		close(command.pipe_fd[1]);
		close(backup_fd[0]);
		close(backup_fd[1]);
		free_minishell(minishell);
	}
	waitpid(command.pid, &status, 0);
	close(command.pipe_fd[1]);
	dup2(command.pipe_fd[0], STDIN);
	close(command.pipe_fd[0]);
	return (status);
}

/* For the firsts pipes, create a pipe and fork the program
 * In the child redirect output into the pipe, execute command and exit.
 * In the parent, close the fd and redirect STDIN. */
int	firsts_pipes(t_shell *minishell, t_simple command, int *backup_fd)
{
	if (pipe(command.pipe_fd) == -1)
	{
		print(2, "minishell: error: pipe failed.\n");
		return (1);
	}
	command.pid = fork();
	if (command.pid == -1)
		return (fork_error());
	else if (command.pid == 0)
	{
		close(command.pipe_fd[0]);
		dup2(command.pipe_fd[1], STDOUT);
		execute_simple_command(minishell, command, backup_fd);
		close(command.pipe_fd[1]);
		close(backup_fd[0]);
		close(backup_fd[1]);
		free_minishell(minishell);
	}
	close(command.pipe_fd[1]);
	dup2(command.pipe_fd[0], STDIN);
	close(command.pipe_fd[0]);
	return (0);
}

/* For each simple command, connect the input and the output to the pipe,
 * except for the last one, the output is on stdout. */
void	create_pipeline(t_command_line *command_line, t_shell *minishell,
		int *backup_fd)
{
	size_t	i;
	int		status;

	i = 0;
	status = 0;
	while (i < command_line->number_of_simple_commands)
	{
		if (i < command_line->number_of_simple_commands - 1)
			firsts_pipes(minishell, command_line->command[i], backup_fd);
		else
			status = last_pipe(minishell, command_line->command[i], backup_fd);
		i++;
	}
	i = 0;
	while (i < command_line->number_of_simple_commands - 1)
	{
		waitpid(command_line->command[i].pid,
			&(command_line->command[i].pid), 0);
		i++;
	}
	if (WIFEXITED(status))
		global_exit_status(WEXITSTATUS(status));
	else if WIFSIGNALED(status)
		global_exit_status(WTERMSIG(status));
}
