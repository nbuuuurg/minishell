/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:09:29 by adeflers          #+#    #+#             */
/*   Updated: 2025/12/09 21:09:29 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd	get_cmd(t_pipeline pipeline, char **path)
{
	t_cmd	cmd;

	ft_bzero(&cmd, sizeof(t_cmd));
	(1 && (cmd.redirect = pipeline.redirect), (cmd.cmd = pipeline.args));
	if (!cmd.cmd)
		return (cmd);
	if (get_cmd_full_path(&cmd, path) != 0)
		return (cmd);
	if (!cmd.full_path)
	{
		if (cmd.cmd[0] && (ft_strncmp(cmd.cmd[0], "./", 2) == 0
				|| ft_strncmp(cmd.cmd[0], "/", 1) == 0))
		{
			cmd.full_path = ft_strdup(cmd.cmd[0]);
			if (!cmd.full_path)
				return (cmd);
		}
		else
		{
			cmd.full_path = ft_strdup("");
			if (!cmd.full_path)
				return (cmd);
		}
	}
	return (cmd);
}

int	get_cmd_full_path(t_cmd *cmd, char **path)
{
	int		i;
	char	*path_cmd;

	i = 0;
	while (path && path[i])
	{
		path_cmd = ft_strjoin(path[i], cmd->cmd[0]);
		if (!path_cmd)
			return (1);
		if (access(path_cmd, F_OK) != -1)
		{
			if (access(path_cmd, X_OK) != -1)
			{
				cmd->full_path = path_cmd;
				break ;
			}
		}
		free(path_cmd);
		i++;
	}
	return (0);
}

void	free_exec_cmd(t_line *line)
{
	free_cmd_path(line);
	free(line->cmd);
	if (line->subline)
	{
		free_line_fork(line, 0);
	}
	else
	{
		if (line->envp)
			free_split(line->envp);
		free_line(line);
	}
}
