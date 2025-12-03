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

void	exec_exprs(t_expr *exprs, char **path, t_line *line)
{

	int		i;
	t_cmd	*cmd;
	int		fd[2];
	int		fd_next[2];
	t_signals	sig;

	handle_signals_child(&sig);
	if (exprs->has_subshell != 0 || line->heredoc_flag == 1)
		return ;
	cmd = malloc(sizeof(t_cmd) * (exprs->pipe_count + 1)); // gerer les erreurs si un truc est NULL // voir si pas mieux t_cmd **cmd
	if (!cmd)
		return ; // error malloc;
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
				{
					if (pipe(fd_next) == -1)
						return (perror("pipe"));
				}
				if (exprs->pipeline[i].position == 1)
					cmd[i].id = exec_cmd(&cmd[i], NULL, fd_next, line);
				else if (exprs->pipeline[i].position == 2)
					cmd[i].id = exec_cmd(&cmd[i], fd, fd_next, line);
				else if (exprs->pipeline[i].position == 3)
					cmd[i].id = exec_cmd(&cmd[i], fd, NULL, line);
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

pid_t exec_cmd(t_cmd *cmd, int *fd_in, int *fd_out, t_line *line)
{
    pid_t	id;
	int		exit_code;
	int		flag_ex_tma = 1;
	struct stat	sb;

	if (cmd->cmd && cmd->cmd[0] && is_builtin(cmd->cmd[0]) == 3 && line->subline == NULL && cmd->pipe_count == 0)
	{
		exit_code = exec_builtin(*cmd, line, 1);
		flag_ex_tma = 0;
	}
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
				if (flag_ex_tma)
					exit_code = exec_builtin(*cmd, line, 0);
				free_exec_cmd(line);
				_exit(exit_code);
			}
			else if (cmd->cmd && cmd->cmd[0])
			{
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

int		get_fd(int *fd_in, int *fd_out, t_redir *redirect, char *cmd)
{
	if (fd_in)
	{
		dup2(fd_in[0], STDIN_FILENO);
		close(fd_in[0]);
		close(fd_in[1]);
	}
	if (fd_out && cmd != NULL)
	{
		dup2(fd_out[1], STDOUT_FILENO);
		close(fd_out[0]);
		close(fd_out[1]);
	}
	else if (fd_out && cmd == NULL)
	{
		close(fd_out[0]);
		close(fd_out[1]);
	}
	if (redirect)
		return (ft_redir(redirect, cmd));
	return (0);
}

int	ft_redir(t_redir *redirect, char *cmd)
{
	int	i;
	int	fd;

	(void)cmd;
	i = 0;
	if (redirect[i].file == NULL)
		return(1);
	while (redirect[i].file)
	{
		if (ft_strncmp(redirect[i].redir, ">", ft_strlen(redirect[i].redir)) == 0)
		{
			fd = open(redirect[i].file, O_WRONLY | O_TRUNC | O_CREAT, 0644); 
			if (fd == -1)
				return (perror(redirect[i].file), 1);
			if (cmd != NULL)
				dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (ft_strncmp(redirect[i].redir, ">>", ft_strlen(redirect[i].redir)) == 0)
		{
			fd = open(redirect[i].file, O_WRONLY | O_APPEND | O_CREAT, 0644); 
			if (fd == -1)
				return (perror(redirect[i].file), 1);
			if (cmd != NULL)
				dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (ft_strncmp(redirect[i].redir, "<", ft_strlen(redirect[i].redir)) == 0)
		{
			fd = open(redirect[i].file, O_RDONLY, 0644);
			if (fd == -1)
				return (perror(redirect[i].file), 1);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (ft_strncmp(redirect[i].redir, "<<", ft_strlen(redirect[i].redir)) == 0)
		{
			fd = redirect[i].hd_fd;
			if (fd == -1)
				return (1); // errror hd_c
			dup2(fd, STDIN_FILENO);
			close(fd);
			redirect[i].hd_fd = -1;
		}
		i++;
	}
	return (0);
}

int	hd_c(char *limiter, t_line *line)
{
	int		here_tube[2];
	char	*content;
	char	*res;
	char	*tmp;
	char	*temp = NULL;
	int		flag;
	int		save_stdin = dup(STDIN_FILENO);
	t_signals	sig;

	handle_signals_hd(&sig);
	res = ft_strdup("");
	if (!res)
		return (perror("malloc"), restore_signals_hd(&sig), -1);
	if (pipe(here_tube) == -1)
		return (perror("pipe"), restore_signals_hd(&sig), free(res), -1);

	flag = 0;
	while (1)
	{
		content = readline("heredoc> ");
		if (g_sig == 1)
		{
			g_sig = 0;
			flag = 1;
			free(content);
			break ;
		}
		if (!content)
		{
			ft_putstr_fd("mini: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
			if (limiter)
				ft_putstr_fd(limiter, STDERR_FILENO);
			ft_putstr_fd("')\n", STDERR_FILENO);
			break ;
		}
		if (ft_strncmp(content, limiter, ft_strlen(limiter)) == 0
			&& content[ft_strlen(limiter)] == '\0')
		{
			free(content);
			break ;
		}
		if (content && need_expand(content) != 0)
		{
			temp = expanded_content(content, line);
			if (!temp)
				return (free(res), free(content), restore_signals_hd(&sig), perror("malloc"), -1);
			free(content);
			content = temp;
		}
		tmp = ft_strjoin(content, "\n");
		if (!tmp)
			return (free(res), free(content), restore_signals_hd(&sig), perror("malloc"), -1);
		free(content);
		content = tmp;
		tmp = res;
		if (!tmp)
			return (free(content), free(res), restore_signals_hd(&sig), perror("malloc"), -1);
		res = ft_strjoin(tmp, content);
		if (!res)
			return (free(content), free(tmp), restore_signals_hd(&sig), perror("malloc"), -1);
		free(content);
		free(tmp);
	}

	if (flag == 1)
	{
		dup2(save_stdin, STDIN_FILENO);
		close(save_stdin);
		close(here_tube[1]);
		free(res);
		restore_signals_hd(&sig);
		line->heredoc_flag = 1;
		line->prev_exit = 130;
		return (here_tube[0]);
	}
	dup2(save_stdin, STDIN_FILENO);
	close(save_stdin);
	write(here_tube[1], res, ft_strlen(res));
	free(res);
	close(here_tube[1]);
	restore_signals_hd(&sig);
	return (here_tube[0]);
}

t_cmd	get_cmd(t_pipeline pipeline, char **path)
{
	int		i;
	t_cmd	cmd;
	char	*path_cmd;

	ft_bzero(&cmd, sizeof(t_cmd));
	cmd.id = -2;
	cmd.redirect = pipeline.redirect;
	cmd.cmd = pipeline.args;
	i = 0;
	if (!cmd.cmd)
	{
		return (cmd);
	}
	/* printf("cmd.cmd[0] = %s\n", cmd.cmd[0]); */
	while (path && path[i])
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
