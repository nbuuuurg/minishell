/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 20:45:16 by adeflers          #+#    #+#             */
/*   Updated: 2025/10/29 20:45:16 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd	*init_cmd(t_expr *exprs, t_line *line, char **path)
{
	int			i;
	t_cmd		*cmd;

	cmd = malloc(sizeof(t_cmd) * (exprs->pipe_count + 1));
	if (!cmd)
		return (NULL);
	i = 0;
	ft_bzero(cmd, sizeof(t_cmd) * (exprs->pipe_count + 1));
	line->cmd = cmd;
	while (i <= exprs->pipe_count)
	{
		if (exprs->has_subshell == 0)
		{
			cmd[i] = get_cmd(exprs->pipeline[i], path);
			cmd[i].pipe_count = exprs->pipe_count;
		}
		i++;
	}
	return (cmd);
}

void prepare_fds_piped(int *fd, int *fd_next, t_expr *exprs, int i)
{
	if (i > 0)
	{
		close(fd[0]);
		close(fd[1]);
	}
	if (exprs->pipeline[i].position != 3)
	{
		fd[0] = fd_next[0];
		fd[1] = fd_next[1];
	}
}

void	handle_pipes(t_expr *exprs, t_cmd *cmd, t_line *line)
{
	int			i;
	int			fd[2];
	int			fd_next[2];

	i = 0;
	while (i <= exprs->pipe_count)
	{
		if (exprs->has_subshell == 0)
		{
			if (exprs->pipe_count == 0)
				cmd[i].id = exec_cmd(&cmd[i], NULL, NULL, line);
			else
			{
				if (exprs->pipeline[i].position != 3)
					if (pipe(fd_next) == -1)
						return (perror("pipe"));
				if (exprs->pipeline[i].position == 1)
					cmd[i].id = exec_cmd(&cmd[i], NULL, fd_next, line);
				else if (exprs->pipeline[i].position == 2)
					cmd[i].id = exec_cmd(&cmd[i], fd, fd_next, line);
				else if (exprs->pipeline[i].position == 3)
					cmd[i].id = exec_cmd(&cmd[i], fd, NULL, line);
				prepare_fds_piped(fd, fd_next, exprs, i);
			}
		}
		i++;
	}
}

void	wait_childs(t_cmd *cmd, t_expr *exprs)
{
	int	i;

	i = 0;
	while (i <= exprs->pipe_count)
	{
		if (exprs->has_subshell == 0)
			waitpid(cmd[i].id, &cmd[i].status, 0);
		i++;
	}
}

void	exec_exprs(t_expr *exprs, char **path, t_line *line)
{
	int			i;
	t_cmd		*cmd;
	t_signals	sig;

	handle_signals_child(&sig);
	if (exprs->has_subshell != 0 || line->heredoc_flag == 1)
		return ;
	cmd = init_cmd(exprs, line, path);
	if (!cmd)
		return ;
	handle_pipes(exprs, cmd, line);
	wait_childs(cmd, exprs);
	i = 0;
	while (i <= exprs->pipe_count)
	{
		if (cmd[i].full_path)
			free(cmd[i].full_path);
		i++;
	}
	if (WIFEXITED(cmd[i - 1].status))
		line->prev_exit = WEXITSTATUS(cmd[i - 1].status);
	else if (WIFSIGNALED(cmd[i - 1].status))
		line->prev_exit = 128 + WTERMSIG(cmd[i - 1].status);
	restore_signals_child(&sig);
	free(cmd);
}
