/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 08:05:30 by adeflers          #+#    #+#             */
/*   Updated: 2025/11/28 09:10:01 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_cmd_path(char **path, t_cmd *cmd)
{
	int		i;
	char	*path_cmd;

	i = 0;
	if (!cmd->cmd[0])
		return ;
	while (path && path[i])
	{
		path_cmd = ft_strjoin(path[i], cmd->cmd[0]);
		if (!path_cmd)
			return (perror("maloc"));
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
}

t_cmd	get_cmd(t_pipeline pipeline, char **path)
{
	t_cmd	cmd;

	ft_bzero(&cmd, sizeof(t_cmd));
	cmd.redirect = pipeline.redirect;
	cmd.cmd = pipeline.args;
	if (!cmd.cmd)
		return (cmd);
	get_cmd_path(path, &cmd);
	if (!cmd.full_path)
	{
		if (cmd.cmd[0] && (ft_strncmp(cmd.cmd[0], "./", 2) == 0
				|| ft_strncmp(cmd.cmd[0], "/", 1) == 0))
		{
			cmd.full_path = ft_strdup(cmd.cmd[0]);
			if (!cmd.full_path)
				return (perror("maloc"), cmd);
		}
		else
		{
			cmd.full_path = ft_strdup("");
			if (!cmd.full_path)
				return (perror("malloc"), cmd);
		}
	}
	return (cmd);
}
