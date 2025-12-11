/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 03:27:17 by adeflers          #+#    #+#             */
/*   Updated: 2025/12/11 03:27:17 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	child_process_builtin(t_cmd *cmd, t_line *line, int exit_code,
								int flag_ex_tma)
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
}

void	check_stat(t_cmd *cmd, t_line *line)
{
	struct stat	sb;

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
}

void	case_execve(t_cmd *cmd, t_line *line)
{
	if (execve(cmd->full_path, cmd->cmd, line->envp) == -1)
	{
		if (errno == ENOENT)
			err_enoent(cmd, line);
		else if (errno == EACCES)
			err_eacces(cmd, line);
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

void	err_enoent(t_cmd *cmd, t_line *line)
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

void	err_eacces(t_cmd *cmd, t_line *line)
{
	ft_putstr_fd(cmd->cmd[0], STDERR_FILENO);
	ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	free_exec_cmd(line);
	clear_history();
	_exit(126);
}
