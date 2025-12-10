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
	cmd = malloc(sizeof(t_cmd) * (exprs->pipe_count + 1));
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
				clear_history();
                _exit(exit_code);
			}
			else if (cmd->cmd && is_builtin(cmd->cmd[0]) == 2)
			{
				free_exec_cmd(line);
				clear_history();
                _exit(exit_code);
			}
			else if (cmd->cmd && is_builtin(cmd->cmd[0]) == 3)
			{
				if (flag_ex_tma)
					exit_code = exec_builtin(*cmd, line, 0);
				free_exec_cmd(line);
				clear_history();
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
						clear_history();
						_exit(127);
					}
					if (S_ISDIR(sb.st_mode))
					{
						ft_putstr_fd(cmd->cmd[0], STDERR_FILENO);
						ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
						free_exec_cmd(line);
						clear_history();
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
						clear_history();
						_exit(127);
					}
					else if (errno == EACCES)
					{
					   ft_putstr_fd(cmd->cmd[0], STDERR_FILENO);
					   ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
					   free_exec_cmd(line);
						clear_history();
					   _exit(126);
					}
					else
					{
					   perror(cmd->cmd[0]);
					   free_exec_cmd(line);
						clear_history();
					   _exit(126);
					}
				}
				else
				{
					perror(cmd->cmd[0]);
					free_exec_cmd(line);
					clear_history();
					_exit(127);
				}
			}
			else if (!cmd->cmd[0])
			{
				free_exec_cmd(line);
				clear_history();
				_exit(0);
			}
			else
			{
				free_exec_cmd(line);
				clear_history();
				_exit(0);
			}
        }
		else
		{
			free_exec_cmd(line);
			if (!g_sig)
			{
				clear_history();
				_exit(1);
			}
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

