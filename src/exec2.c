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
	while (line->exprs != NULL)
	{
		exec_exprs(line->exprs, line->path, env);
		line->exprs = line->exprs->next;
	}
}

void	exec_exprs(t_expr *exprs, char **path ,char **env)
{
	int		i;
	t_cmd	*cmd;
	int		fd[2];
	int		fd_next[2];

	cmd = malloc(sizeof(t_cmd) * (exprs->pipe_count + 1)); // gerer les erreurs si un truc est NULL
	if (!cmd)
		return ; // error malloc;
	i = 0;
	while (i <= exprs->pipe_count)
	{
		cmd[i] = get_cmd(exprs->pipeline[i], path, env);
		i++;
	}
	i = 0;
	while (i <= exprs->pipe_count)
	{
		if (exprs->pipe_count == 0)
			cmd[i].id = exec_cmd(cmd[i], NULL, NULL);
		else
		{
			if (exprs->pipeline[i].position != 3)
			{
				if (pipe(fd_next) == -1)
					return (perror("pipe"));
			}
			if (exprs->pipeline[i].position == 1)
				cmd[i].id = exec_cmd(cmd[i], NULL, fd_next);
			else if (exprs->pipeline[i].position == 2)
				cmd[i].id = exec_cmd(cmd[i], fd, fd_next);
			else if (exprs->pipeline[i].position == 3)
				cmd[i].id = exec_cmd(cmd[i], fd, NULL);
			/* if (cmd[i].redirect && ft_strncmp(cmd[i].redirect->redir, ">", 1) == 0) */
			/* 	close(fd_next[1]); */
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
		i++;
		/* if (exprs->pipe_count > 0) */
		/* { */
		/* 	close(fd[0]); */
		/* 	close(fd[1]); */
		/* } */
	}
	i = 0;
	while (i <= exprs->pipe_count)
	{
		waitpid(cmd[i].id, &cmd[i].status, 0);
		i++;
	}
	free(cmd);
}

pid_t	exec_cmd(t_cmd cmd, int *fd_in, int *fd_out) // ajouter gestion buildin
{
	pid_t	id;

	id = fork();
	if (id == -1)
		return (perror("fork"),	id);// error fork
	if (id == 0)
	{
		if (get_fd(fd_in, fd_out, cmd.redirect) == 0)
		{
			if (cmd.cmd[0])
			{
				if (cmd.full_path)
				{
					execve(cmd.full_path, cmd.cmd, cmd.env);
					perror("execve");
				}
				else 
				{
					ft_putstr_fd("command not found\n", 2);
					exit (127);
				}
			}
		
		}
		exit (1); // penser a free cmd
	}
	return (id);
}

int		get_fd(int *fd_in, int *fd_out, t_redir *redirect) // on gerera les redir ici
{
	if (fd_in)
	{
		
		dup2(fd_in[0], STDIN_FILENO);
		close(fd_in[0]);
		close(fd_in[1]);
	}
	if (fd_out)
	{
		dup2(fd_out[1], STDOUT_FILENO);
		close(fd_out[0]);
		close(fd_out[1]);
	}
	if (redirect)
	{
		return (ft_redir(redirect, fd_in, fd_out));
	}
	return (0);
}

// Attention : prb redir et pipes et ajouter des close apres les dup2 et gerer qd plusieurs redirections pour une seule commande 

int	ft_redir(t_redir *redirect, int fd_in[2], int fd_out[2])
{
	int	i;
	int	fd;

	i = 0;
	while (redirect[i].file)
	{
		/* printf("%d\n", i); */
		/* printf("%s\n", redirect[i].file); */
		if (ft_strncmp(redirect[i].redir, ">", ft_strlen(redirect[i].redir)) == 0)
		{
			fd = open(redirect[i].file, O_WRONLY | O_TRUNC | O_CREAT, 0644); 
			dup2(fd, STDOUT_FILENO);
			close(fd);
			if (fd_out)
			{
				dup2(fd_out[1], STDIN_FILENO);
				close(fd_out[0]);
				close(fd_out[1]);
				fd_out = NULL;
			}
			if (fd_in)
			{
				dup2(fd_in[0], STDIN_FILENO);
				close(fd_in[0]);
				close(fd_in[1]);
			}
		}
		else if (ft_strncmp(redirect[i].redir, ">>", ft_strlen(redirect[i].redir)) == 0)
		{
			fd = open(redirect[i].file, O_WRONLY | O_APPEND | O_CREAT, 0644); 
			dup2(fd, STDOUT_FILENO);
			close(fd);
			if (fd_out)
			{
				dup2(fd_out[1], STDIN_FILENO);
				close(fd_out[0]);
				close(fd_out[1]);
				fd_out = NULL;
			}
			if (fd_in)
			{
				dup2(fd_in[0], STDIN_FILENO);
				close(fd_in[0]);
				close(fd_in[1]);
			}
		}
		else if (ft_strncmp(redirect[i].redir, "<", ft_strlen(redirect[i].redir)) == 0) // ok
		{
			fd = open(redirect[i].file, O_RDONLY, 0644); // a voir au parsing si ya rien 
			if (fd == -1)
				return (perror("mini"), 1); // la cmd au lieu de mini
			dup2(fd, STDIN_FILENO);
			close(fd);
			if (fd_out)
			{
				dup2(fd_out[1], STDOUT_FILENO);
				close(fd_out[0]);
				close(fd_out[1]);
				fd_out = NULL;
			}
			if (fd_in)
			{
				dup2(fd_in[0], STDOUT_FILENO);
				close(fd_in[0]);
				close(fd_in[1]);
			}
		}
		i++;
	}
	return (0);

	/* else if (ft_strncmp(redirect->redir, "<<", ft_strlen(redirect->redir)) == 0) // here_doc */
	/* { */
	/* 	fd = open(redirect->file, O_RDONLY, 0644);  */
	/* 	dup2(fd, STDIN_FILENO); */
	/* 	return (0); */
	/* } */
	return (-1);

}


t_cmd	get_cmd(t_pipeline pipeline, char **path, char **env)
{
	int		i;
	t_cmd	cmd;
	char	*path_cmd;

	ft_bzero(&cmd, sizeof(t_cmd));
	/* printf("%p\n", pipeline.redirect); */
	cmd.redirect = pipeline.redirect;
	// si c est un buildin --> on met buildin = 1
	// else 
	// buildin = 0 
	// et on fait le reste
	cmd.cmd = pipeline.args;
	cmd.env = env;
	if (!cmd.env)
		cmd.full_path = ft_strdup(cmd.cmd[0]);
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
