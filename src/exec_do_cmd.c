/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_do_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 10:02:46 by adeflers          #+#    #+#             */
/*   Updated: 2025/11/28 10:02:46 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exec_builtin_child(t_cmd *cmd, t_line *line, int exit_code)
{
	if (cmd->cmd && is_builtin(cmd->cmd[0]) == 1)
	{
		exit_code = exec_builtin(*cmd, line, 0);
		free_exec_cmd(line);
		_exit(exit_code);
	}
	else if (cmd->cmd && is_builtin(cmd->cmd[0]) == 2)
	{
		free_exec_cmd(line);
		_exit(exit_code);
	}
	else if (cmd->cmd && is_builtin(cmd->cmd[0]) == 3)
	{
		exit_code = exec_builtin(*cmd, line, 0);
		free_exec_cmd(line);
		_exit(exit_code);
	}

}

void error_1(t_cmd *cmd, t_line *line)
{
	struct stat		sb;

	if (stat(cmd->full_path, &sb) == 0)
	{
		if (cmd->cmd[0][0] == '\0')
		{
			ft_putstr_fd(cmd->cmd[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			free_exec_cmd(line);
			_exit(127);
		}
		if (S_ISDIR(sb.st_mode))
		{
			ft_putstr_fd(cmd->cmd[0], STDERR_FILENO);
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
			free_exec_cmd(line);
			_exit(126);
		}
	}
}

void close_heredoc_fds(t_line *line)
{
	int	i;
	int	j;

	i = line->exprs->pipe_count;
	while (i >= 0)
	{
		j = 0;
		while (j < line->exprs->pipeline[i].redir_count)
		{
			if (ft_strncmp(line->exprs->pipeline[i].redirect[j].redir, "<<", 2) == 0)
			{
				if (line->exprs->pipeline[i].redirect[j].hd_fd != -1)
				{
					close(line->exprs->pipeline[i].redirect[j].hd_fd);
					line->exprs->pipeline[i].redirect[j].hd_fd = -1;
				}
			}
			j++;
		}
		i--;
	}
}

pid_t	exec_cmd(t_cmd *cmd, int *fd_in, int *fd_out, t_line *line)
{
	pid_t			id;
	int				exit_code;

	if (cmd->cmd && cmd->cmd[0] && is_builtin(cmd->cmd[0]) == 3
		&& line->subline == NULL && cmd->pipe_count == 0)
		exit_code = exec_builtin(*cmd, line, 1);
	if (cmd->cmd && cmd->cmd[0] && is_builtin(cmd->cmd[0]) == 2)
		exit_code = exec_builtin(*cmd, line, 0);
	id = fork();
	if (id == -1)
		return (perror("fork"), id);
	if (id == 0)
	{
		setup_signals_child();
		if (get_fd(fd_in, fd_out, cmd->redirect, cmd->cmd[0]) == 0)
		{
			exec_builtin_child(cmd, line, exit_code);
			if (cmd->cmd && cmd->cmd[0])
			{
				error_1(cmd, line);
				close_heredoc_fds(line);
				if (execve(cmd->full_path, cmd->cmd, line->envp) == -1)
				{
					if (errno == ENOENT)
					{
						if (ft_strchr(cmd->cmd[0], '/') == 0)
						{
							ft_putstr_fd(cmd->cmd[0], STDERR_FILENO);
							ft_putstr_fd(": command not found\n", STDERR_FILENO);
						}
						else
						{
							ft_putstr_fd(cmd->cmd[0], STDERR_FILENO);
							ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
						}
						free_exec_cmd(line);
						_exit(127);
					}
					else if (errno == EACCES)
					{
						ft_putstr_fd(cmd->cmd[0], STDERR_FILENO);
						ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
						free_exec_cmd(line);
						_exit(126);
					}
					else
					{
						perror(cmd->cmd[0]);
						free_exec_cmd(line);
						_exit(126);
					}
				}
				else
				{
					perror(cmd->cmd[0]);
					free_exec_cmd(line);
					_exit(127);
				}
			}
			else if (!cmd->cmd[0])
			{
				free_exec_cmd(line);
				_exit(0);
			}
			else
			{
				free_exec_cmd(line);
				_exit(0);
			}
		}
		else
		{
			free_exec_cmd(line);
			if (!g_sig)
				_exit(1);
		}
	}
	else
	{
		if (cmd->redirect)
		{
			int	i = 0;
			while (cmd->redirect[i].redir)
			{
				if (ft_strncmp(cmd->redirect[i].redir, "<<", 3) == 0)
				{
					if (cmd->redirect[i].hd_fd != -1)
					{
						close(cmd->redirect[i].hd_fd);
						cmd->redirect[i].hd_fd = -1;
					}
				}
				i++;
			}
		}
	}
	return (id);
}
