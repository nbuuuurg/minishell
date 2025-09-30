/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:44:26 by adeflers          #+#    #+#             */
/*   Updated: 2025/09/29 16:44:26 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

void	exec_minishell2(t_line *line, char **env)
{
	/* int	i; */
	/* int	j; */

	while (line->exprs != NULL)
	{
		/* printf("nb de pipe : %d\n", line->exprs->pipe_count); */
		/* j = 0; */
		/* while (j <= line->exprs->pipe_count) */
		/* { */
		/* 	i = -1; */
		/* 	while (line->exprs->pipeline[j].args[++i]) */
		/* 		printf("%s\n", line->exprs->pipeline[j].args[i]); */
		/* 	printf("%s\n\n", line->exprs->pipeline[j].args[i]); */
		/* 	j++; */
		/* } */

		exec_exprs(line->exprs, line->path, env);

		line->exprs = line->exprs->next;
	}
}

void	exec_exprs(t_expr *exprs, char **path ,char **env)
{
	if (exprs->pipe_count == 0)
		single_cmd(exprs, path, env);
}

void	single_cmd(t_expr *exprs, char **path, char **env)
{
	t_cmd	cmd;

	// if is_buildin
	// --> exec buildin
	// else 
	cmd = get_cmd(exprs->pipeline->args, path, env);
	cmd.id = exec_cmd(cmd, env);

	waitpid(cmd.id, &cmd.status, 0);
}

pid_t	exec_cmd(t_cmd cmd, char **env)
{
	pid_t	id;

	id = fork();
	if (id == -1)
		return (id);// error fork
	if (id == 0)
		execve(cmd.full_path, cmd.cmd, env);
	return (id);
}

t_cmd	get_cmd(char **args, char **path, char **env)
{
	int		i;
	t_cmd	cmd;
	char	*path_cmd;

	(void)env;
	cmd.cmd = args;
	i = 0;
	while (path[i])
	{
		path_cmd = ft_strjoin(path[i], cmd.cmd[0]);
		if (!path_cmd)
			return (cmd); // errro malloc
		if (access(path_cmd, F_OK) != -1)
		{
			if (access(path_cmd, X_OK) != -1)
			{
				cmd.full_path = path_cmd;
				break ;
			}
		}
		free(path_cmd);
		i++;
	}	
	return (cmd);
}
