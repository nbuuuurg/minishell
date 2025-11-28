/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 08:05:30 by adeflers          #+#    #+#             */
/*   Updated: 2025/11/28 08:25:08 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_cmd_path(char **path, t_cmd *cmd)
{
	int		i;
	char	*path_cmd;

	i = 0;
	while (path && path[i])
	{
		path_cmd = ft_strjoin(path[i], cmd->cmd[0]);
		if (!path_cmd)
			return ; // errro malloc
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
	return ;

}

t_cmd	get_cmd(t_pipeline pipeline, char **path)
{
	int		i;
	t_cmd	cmd;

	ft_bzero(&cmd, sizeof(t_cmd));
	cmd.id = -2;
	cmd.redirect = pipeline.redirect;
	cmd.cmd = pipeline.args;
	i = 0;
	if (!cmd.cmd)
		return (cmd);
	get_cmd_path(path, &cmd);
	if (!cmd.full_path)
	{
		if (cmd.cmd[0] && (ft_strncmp(cmd.cmd[0], "./", 2) == 0 || ft_strncmp(cmd.cmd[0], "/", 1) == 0))
		{
			cmd.full_path = ft_strdup(cmd.cmd[0]);
			if (!cmd.full_path)
				return (cmd); // error malloc
		}
		else
		{
			cmd.full_path = ft_strdup("");
			if (!cmd.full_path)
				return (cmd); // error malloc
		}

	}
	return (cmd);
}
