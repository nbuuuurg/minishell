#include "../include/minishell.h"

#include "../include/minishell.h"

void	exec_minishell(t_line *line)
{
	/*ici*/
	t_expr	*temp;
	temp = line->exprs;
	while (temp != NULL)
	{
		if (temp)
			exec_exprs(temp, line->path, line->envp, line);
		temp = temp->next;
	}
}

void	exec_exprs(t_expr *exprs, char **path ,char **env, t_line *line)
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
		if (exprs->has_subshell == 0)
			cmd[i] = get_cmd(exprs->pipeline[i], path, env);
		i++;
	}
	i = 0;
	while (i <= exprs->pipe_count)
	{
		if (exprs->has_subshell == 0)
		{
			if (exprs->pipe_count == 0)
				cmd[i].id = exec_cmd(cmd[i], NULL, NULL, line);
			else
			{
				if (exprs->pipeline[i].position != 3)
				{
					if (pipe(fd_next) == -1)
						return (perror("pipe"));
				}
				if (exprs->pipeline[i].position == 1)
					cmd[i].id = exec_cmd(cmd[i], NULL, fd_next, line);
				else if (exprs->pipeline[i].position == 2)
					cmd[i].id = exec_cmd(cmd[i], fd, fd_next, line);
				else if (exprs->pipeline[i].position == 3)
					cmd[i].id = exec_cmd(cmd[i], fd, NULL, line);
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
		}
		i++;
	}
	i = 0;
	while (i <= exprs->pipe_count)
	{
		if (exprs->has_subshell == 0)
			waitpid(cmd[i].id, &cmd[i].status, 0);
		i++;
	}
	free(cmd->full_path);
	free(cmd);
}

pid_t	exec_cmd(t_cmd cmd, int *fd_in, int *fd_out, t_line *line) // ajouter gestion buildin
{
	pid_t	id;

	// is buildin qui modifie l env
	
	if (is_builtin(cmd.cmd[0]) == 2)
	{
		/* if (get_fd(fd_in, fd_out, cmd.redirect, here_doc_fds) == 0) */
		exec_builtin(cmd, line);
	}
	id = fork();
	if (id == -1)
		return (perror("fork"),	id); // error fork
	if (id == 0)
	{
		if (get_fd(fd_in, fd_out, cmd.redirect) == 0)
		{
			if (is_builtin(cmd.cmd[0]) == 1)
			{
				exec_builtin(cmd, line);
				// free tt
				exit (0);
			}
			if (is_builtin(cmd.cmd[0]) == 2)
			{
				// free tt
				exit (0);
			}
			if (cmd.cmd[0])
			{
				/* if (cmd.full_path) */
				/* { */
					execve(cmd.full_path, cmd.cmd, cmd.env);
					/* printf("---%s---\n", cmd.full_path); */
					/* perror("execve"); */
				/* } */
				/* else  */
				/* { */
					/* ft_putstr_fd(cmd.cmd[0], 2); */
					/* ft_putstr_fd(": command not found\n", 2); */
					perror(cmd.cmd[0]);
					exit (127);
				/* } */
			}
		}
		exit (1); // penser a free cmd
	}
	return (id);
}

int		get_fd(int *fd_in, int *fd_out, t_redir *redirect)
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
		return (ft_redir(redirect));
	return (0);
}

int	ft_redir(t_redir *redirect)
{
	int	i;
	int	fd;

	i = 0;
	while (redirect[i].file)
	{
		if (ft_strncmp(redirect[i].redir, ">", ft_strlen(redirect[i].redir)) == 0)
		{
			fd = open(redirect[i].file, O_WRONLY | O_TRUNC | O_CREAT, 0644); 
			if (fd == -1)
				return (perror("mini"), 1); // la cmd au lieu de mini
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (ft_strncmp(redirect[i].redir, ">>", ft_strlen(redirect[i].redir)) == 0)
		{
			fd = open(redirect[i].file, O_WRONLY | O_APPEND | O_CREAT, 0644); 
			if (fd == -1)
				return (perror("mini"), 1); // la cmd au lieu de mini
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (ft_strncmp(redirect[i].redir, "<", ft_strlen(redirect[i].redir)) == 0)
		{
			fd = open(redirect[i].file, O_RDONLY, 0644);
			if (fd == -1)
				return (perror("mini"), 1); // la cmd au lieu de mini
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (ft_strncmp(redirect->redir, "<<", ft_strlen(redirect->redir)) == 0)
		{
			fd = redirect->heredoc_fd;
			if (fd == -1)
				return (1); // errror here_doc_content
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		i++;
	}
	return (0);
}

int	here_doc_content(char *limiter, t_line *line)
{
	int		here_tube[2];
	char	*content;
	char	*res;
	char	*tmp;

	res = ft_strdup("");
	if (!res)
		return (perror("malloc"), -1);
	if (pipe(here_tube) == -1)
		return (perror("pipe"), -1);
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		content = get_next_line(STDIN_FILENO);
		if (!content)
			return (free(res), perror("malloc"), -1);
		if (ft_strncmp(content, limiter, ft_strlen(limiter)) == 0
			&& content[ft_strlen(limiter)] == '\n')
		{
			free(content);
			break ;
		}
		(void)line;
		/*ici*/
		while (need_expand(content) != 0)
		{
			// printf("content : %s\n", content);
			content = expanded_content(content, line);
			if (!content)
				return (free(res), perror("malloc"), -1);
		}
		tmp = res;
		if (!tmp)
			return (free(content), free(res), perror("malloc"), -1);
		res = ft_strjoin(tmp, content);
		if (!res)
			return (free(content), free(tmp), perror("malloc"), -1);
		free(content);
		free(tmp);
	}
	write(here_tube[1], res, ft_strlen(res));
	free(res);
	close(here_tube[1]);
	return (here_tube[0]);
}

t_cmd	get_cmd(t_pipeline pipeline, char **path, char **env)
{
	int		i;
	t_cmd	cmd;
	char	*path_cmd;

	ft_bzero(&cmd, sizeof(t_cmd));
	cmd.redirect = pipeline.redirect;
	cmd.cmd = pipeline.args;
	cmd.env = env;
	i = 0;
	path_cmd = NULL;
	while (path && path[i])
	{
		path_cmd = ft_strjoin(path[i], cmd.cmd[0]);
		if (!path_cmd)
			return (free(path_cmd), cmd); // errro malloc
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
	if (!cmd.full_path)
	{
		cmd.full_path = ft_strdup(cmd.cmd[0]);
		if (!cmd.full_path)
			return (cmd); // error malloc
	}
	return (cmd);
}