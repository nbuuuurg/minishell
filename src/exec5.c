/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 04:07:43 by adeflers          #+#    #+#             */
/*   Updated: 2025/12/10 04:07:43 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	child_process(t_cmd *cmd, t_line *line, int exit_code, int flag_ex_tma)
{
	if (cmd->cmd && is_builtin(cmd->cmd[0]) > 0)
		child_process_builtin(cmd, line, exit_code, flag_ex_tma);
	else if (cmd->cmd && cmd->cmd[0])
	{
		check_stat(cmd, line);
		close_hd_fds_in_child(line);
		case_execve(cmd, line);
	}
	else if (!cmd->cmd[0])
	{
		free_exec_cmd(line);
		clear_history();
		_exit(0);
	}
	else
	{
		free_exec_cmd(line);
		clear_history();
		_exit(0);
	}
}

pid_t	exec_cmd(t_cmd *cmd, int *fd_in, int *fd_out, t_line *line)
{
	pid_t	id;
	int		exit_code;
	int		flag_ex_tma;

	flag_ex_tma = 1;
	if (cmd->cmd && cmd->cmd[0] && is_builtin(cmd->cmd[0]) == 3
		&& line->subline == NULL && cmd->pipe_count == 0)
		(1 && (exit_code = exec_builtin(*cmd, line, 1)), (flag_ex_tma = 0));
	if (cmd->cmd && cmd->cmd[0] && is_builtin(cmd->cmd[0]) == 2)
		exit_code = exec_builtin(*cmd, line, 0);
	id = fork();
	if (id == -1)
		return (perror("fork"), id);
	if (id == 0)
	{
		setup_signals_child();
		if (get_fd(fd_in, fd_out, cmd->redirect, cmd->cmd[0]) == 0)
			child_process(cmd, line, exit_code, flag_ex_tma);
		else
			default_exit_path(line);
	}
	close_hd_fds_parent(cmd);
	return (id);
}

void	default_exit_path(t_line *line)
{
	free_exec_cmd(line);
	if (!g_sig)
	{
		clear_history();
		_exit(1);
	}
}

void	close_hd_fds_parent(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->redirect)
	{
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

void	close_hd_fds_in_child(t_line *line)
{
	int	i;
	int	j;

	i = line->exprs->pipe_count;
	while (i >= 0)
	{
		j = 0;
		while (j < line->exprs->pipeline[i].redir_count)
		{
			if (ft_strncmp(line->exprs->pipeline[i].redirect[j].redir,
					"<<", 2) == 0)
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
