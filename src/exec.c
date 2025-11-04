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

int		last_parse_err(t_line *line)
{
	t_token	*temp;
	t_token *temp2;

	temp = line->tokens;
	temp2 = line->tokens;
	while (temp && temp->next)
		temp = temp->next;
	while (temp2 && temp2->previous)
		temp2 = temp2->previous;
	// printf("temp->s = %s\n", temp2->s);
	if (temp && temp->type == PIPE)
	{
		printf("fini par un pipe\n");
		return (line->lexer_err = -1, line->prev_exit = 2, 1);
	}
	if (temp2 && temp2->type == PIPE)
	{
		printf("commence par un pipe\n");
		return (line->lexer_err = -2, line->prev_exit = 2, 1);
	}
	if (line->lexer_err == -3) // erreur subshell
		return (1);
	if (line->lexer_err == -4)
	{
		printf("fini ou commence par && ou ||\n");
		return (line->lexer_err = -3, line->prev_exit = 2, 1);
	}
	if (line->lexer_err == -5)
	{
		printf("erreur redirect.file = NULL\n");
		return (line->lexer_err = -4, line->prev_exit = 2, 1);
	}
	return (0);
}

void	exec_minishell(t_line *line)
{
	/*ici*/
	/* print_expr(line); */
	/* print_token(line); */
	t_expr	*temp;
	temp = line->exprs;
	while(line->tokens->previous)
		line->tokens = line->tokens->previous;
	// printf("err : %d\n", line->lexer_err);
	if (last_parse_err(line))
		return ;
	while (temp != NULL)
	{
		if (temp)
			exec_exprs(temp, line->path, line->envp, line);
		// printf("exit : %d\n", line->prev_exit);
		if (temp)
		{
			if (line->prev_exit == 2) // error redirection
				return ;
			if (line->prev_exit == 0 && temp->op_after == AND) // true &&
				temp = temp->next;
			else if (line->prev_exit != 0 && temp->op_after == AND) // false &&
			{
				while (temp && temp->op_after != OR)
					temp = temp->next;
				if (temp)
					temp = temp->next;
			}
			else if (line->prev_exit != 0 && temp->op_after == OR) // false ||
				temp = temp->next;
			else if (line->prev_exit == 0 && temp->op_after == OR) // true ||
			{
				while (temp && temp->op_after != AND)
					temp = temp->next;
				if (temp)
					temp = temp->next;
			}
			else
				temp = temp->next;
		}
	}
}

void	exec_exprs(t_expr *exprs, char **path ,char **env, t_line *line)
{

	int		i;
	t_cmd	*cmd;
	int		fd[2];
	int		fd_next[2];

	struct sigaction old_int;
	struct sigaction old_quit;
	struct sigaction new;
	sigemptyset(&new.sa_mask);
	new.sa_handler = SIG_IGN;
	new.sa_flags = 0;

	if (exprs->has_subshell != 0)
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
			cmd[i] = get_cmd(exprs->pipeline[i], path, env);
			cmd[i].pipe_count = exprs->pipe_count;
		}
		i++;
	}
	i = 0;
	sigaction(SIGINT, &new, &old_int);
	sigaction(SIGQUIT, &new, &old_quit);
	signal(SIGINT, sigint_handler_child);
	signal(SIGQUIT, sigquit_handler_child);	
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
	//ici
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
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	free(cmd);
}

void	free_exec_cmd(t_line *line)
{
	free_cmd_path(line);
	if (line->envp)
		free_split(line->envp);
	free(line->cmd);
	if (line->subline)
	{
		free_line_fork(line, 0);
	}
	else
		free_line(line);
}

pid_t exec_cmd(t_cmd *cmd, int *fd_in, int *fd_out, t_line *line)
{
    pid_t id;
	/**/
	/* // AVANT l'exécution des redirections */
	/* int saved_stdout = dup(STDOUT_FILENO);  // Sauvegarder STDOUT */
	/* int saved_stdin = dup(STDIN_FILENO);    // Sauvegarder STDIN */
	/**/
	/* // Exécuter les redirections et commandes */
	/* // ... votre code d'exécution ... */

    if (cmd->cmd && cmd->cmd[0] && is_builtin(cmd->cmd[0]) == 2) // cd a pas fork si pas pipe
	{
        exec_builtin(*cmd, line);
    }

    id = fork();
    if (id == -1)
        return (perror("fork"), id);
    if (id == 0)
	{
        if (get_fd(fd_in, fd_out, cmd->redirect, cmd->cmd[0]) == 0) 
		{
            if (cmd->cmd && is_builtin(cmd->cmd[0]) == 1) 
			{
                exec_builtin(*cmd, line);
				free_exec_cmd(line);
                _exit(0);
			}
			else if (cmd->cmd && is_builtin(cmd->cmd[0]) == 2)
			{
				free_exec_cmd(line);
                _exit(0);
			}
			else if (cmd->cmd && cmd->cmd[0])
			{
                execve(cmd->full_path, cmd->cmd, cmd->env);
                perror(cmd->cmd[0]);
				free_exec_cmd(line);
                _exit(127);
            }
			else if (!cmd->cmd[0])
			{
				free_exec_cmd(line);
				/* printf("exit child no cmd\n"); */
				_exit(0);
			}
			else
			{
				//pour etre sur
				free_exec_cmd(line);
				_exit(0);
			}
        }
		else
		{
			free_exec_cmd(line);
			if (!g_sig)
				_exit(2);
		}
    }
	/**/
	/* // APRÈS l'exécution (TOUJOURS, même en cas d'erreur) */
	/* dup2(saved_stdout, STDOUT_FILENO);      // Restaurer STDOUT */
	/* dup2(saved_stdin, STDIN_FILENO);        // Restaurer STDIN */
	/* close(saved_stdout); */
	/* close(saved_stdin); */
	/**/
    return (id);
}

// pid_t	exec_cmd(t_cmd cmd, int *fd_in, int *fd_out, t_line *line) // ajouter gestion buildin
// {
// 	pid_t	id;

// 	// is buildin qui modifie l env
	
// 	if (is_builtin(cmd.cmd[0]) == 2)
// 	{
// 		/* if (get_fd(fd_in, fd_out, cmd.redirect, here_doc_fds) == 0) */
// 		exec_builtin(cmd, line);
// 	}
// 	id = fork();
// 	if (id == -1)
// 		return (perror("fork"),	id); // error fork
// 	if (id == 0)
// 	{
// 		if (get_fd(fd_in, fd_out, cmd.redirect) == 0)
// 		{
// 			if (is_builtin(cmd.cmd[0]) == 1)
// 			{
// 				exec_builtin(cmd, line);
// 				// free tt
// 				exit (0);
// 			}
// 			if (is_builtin(cmd.cmd[0]) == 2)
// 			{
// 				// free tt
// 				exit (0);
// 			}
// 			if (cmd.cmd[0])
// 			{
// 				/* if (cmd.full_path) */
// 				/* { */
// 					execve(cmd.full_path, cmd.cmd, cmd.env);
// 					/* printf("---%s---\n", cmd.full_path); */
// 					/* perror("execve"); */
// 				/* } */
// 				/* else  */
// 				/* { */
// 					/* ft_putstr_fd(cmd.cmd[0], 2); */
// 					/* ft_putstr_fd(": command not found\n", 2); */
// 					perror(cmd.cmd[0]);
// 					exit (127);
// 				/* } */
// 			}
// 		}
// 		exit (1); // penser a free cmd
// 	}
// 	return (id);
// }

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

	// Il faut sortir avec la bonne erreur si redirect[i].file == NULL mais ca ne segault plus

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
	char	*temp = NULL;
	int		flag;

	struct sigaction old_int;
	struct sigaction old_quit;
	struct sigaction sa_int;
	struct sigaction sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sigemptyset(&sa_quit.sa_mask);
	sa_int.sa_flags = 0;
	sa_quit.sa_flags = 0;
	sa_int.sa_handler = sigint_handler_hd;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_int, &old_int);
	sigaction(SIGQUIT, &sa_quit, &old_quit);
	//empeche ^\ avec CTRL '\'
	struct termios 	t;
	struct termios	old;
	tcgetattr(STDIN_FILENO, &old);
	t = old;
	t.c_lflag &= ~ECHOCTL; // désactive le bit ECHOCTL -> n'affiche plus ^/, ^C, etc
	tcsetattr(STDIN_FILENO, TCSANOW, &t);

	res = ft_strdup("");
	if (!res)
		return (perror("malloc"), -1);
	if (pipe(here_tube) == -1)
		return (perror("pipe"), -1);
	flag = 0;
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		content = get_next_line(STDIN_FILENO);
		if (g_sig == 1)
		{
			g_sig = 0;
			flag = 1;
			break ;
		}
		if (!content)
		{
				write(STDERR_FILENO,"\nhere-document delimited by end-of-file\n", 40);
				break;
				// return (free(res), perror("malloc"), -1);  // a differencier avec EOF (CTRL D) et erreur malloc
		}
		if (ft_strncmp(content, limiter, ft_strlen(limiter)) == 0
			&& content[ft_strlen(limiter)] == '\n')
		{
			free(content);
			break ;
		}
		(void)line;
		/*ici*/
		while (content && need_expand(content) != 0)
		{
			temp = expanded_content(content, line);
			if (!temp)
				return (free(res), free(content), perror("malloc"), -1);
			free(content);
			content = temp;
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
	if (flag == 1)
	{
		close(here_tube[1]);
		close(here_tube[0]);
		free(res);
		sigaction(SIGINT, &old_int, NULL);
		sigaction(SIGQUIT, &old_quit, NULL);
		tcsetattr(STDIN_FILENO, TCSANOW, &old);
		line->heredoc_flag = 1;
		line->prev_exit = 130;
		return (-2);
	}
	write(here_tube[1], res, ft_strlen(res));
	free(res);
	close(here_tube[1]);
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
	return (here_tube[0]);
}

t_cmd	get_cmd(t_pipeline pipeline, char **path, char **env)
{
	int		i;
	t_cmd	cmd;
	char	*path_cmd;

	ft_bzero(&cmd, sizeof(t_cmd));
	cmd.id = -2;
	cmd.redirect = pipeline.redirect;
	cmd.cmd = pipeline.args;
	cmd.env = env;
	i = 0;
	if (!cmd.cmd)
	{
		/* printf("cmd.cmd NULL\n"); */
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
		cmd.full_path = ft_strdup(cmd.cmd[0]);
		if (!cmd.full_path)
			return (cmd); // error malloc
	}
	return (cmd);
}
