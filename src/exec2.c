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

void	exec_minishell(t_line *line, char **env)
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
	int		i;
	t_cmd	*cmd;
	int		fd[2];
	/* int		fd_next[2]; */

	cmd = malloc(sizeof(t_cmd) * (exprs->pipe_count + 1));
	if (!cmd)
		return ; // error malloc;
	i = 0;
	while (i <= exprs->pipe_count)
	{
		cmd[i] = get_cmd(exprs->pipeline[i].args, path, env);
		i++;
	}
	if (exprs->pipe_count > 0)
		if (pipe(fd) == -1)
			return (perror("pipe"));
	i = 0;
	while (i <= exprs->pipe_count)
	{
		/* if (exprs->pipeline[i].position == 2) */
		/* { */
		/* 	if (pipe(fd_next) == -1) */
		/* 		return (perror("pipe")); */
		/* } */
		exec_process(cmd[i], env, exprs->pipeline[i].position, fd);
		if (exprs->pipe_count > 0 && exprs->pipeline[i].position != 1)
		{
			close(fd[0]);
			close(fd[1]);
		}
		/* if (i < exprs->pipe_count) */
		/* { */
		/* 	fd[0] = fd_next[0]; */
		/* 	fd[1] = fd_next[1]; */
		/* } */
		i++;
	}
	i = 0;
	while (i <= exprs->pipe_count)
	{
		waitpid(cmd[i].id, &cmd[i].status, 0);
		i++;
	}
	free(cmd);
}

void	exec_process(t_cmd cmd, char **env, t_pipe position, int *fd)
{
	printf("%s : position du pipe : %d\n", cmd.cmd[0], position);
	if (position == 0)
		cmd.id = exec_cmd(cmd, env, position, fd);
	else
	{
		cmd.id = exec_cmd(cmd, env, position, fd);
	}
}

pid_t	exec_cmd(t_cmd cmd, char **env, t_pipe position, int *fd)
{
	pid_t	id;

	id = fork();
	if (id == -1)
		return (perror("fork"),	id);// error fork
	if (id == 0)
	{
		/* if (cmd.buildin) */
		/* 	exec_buildin(cmd.cmd_buildin); avec un exit a la fin */
		/* else */
		if (position == 0)
			execve(cmd.full_path, cmd.cmd, env);
		else
		{
			get_fd(position, fd);
			execve(cmd.full_path, cmd.cmd, env);
		}
	}
	return (id);
}

void	get_fd(t_pipe position, int *fd) // on gerera les redir ici
{
	if (position == 1)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	if (position == 3)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		close(fd[0]);
	}
}

t_cmd	get_cmd(char **args, char **path, char **env)
{
	int		i;
	t_cmd	cmd;
	char	*path_cmd;

	(void)env;
	ft_bzero(&cmd, sizeof(t_cmd));
	// si c est un buildin --> on met buildin = 1
	// else 
	// buildin = 0 
	// et on fait le reste
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
