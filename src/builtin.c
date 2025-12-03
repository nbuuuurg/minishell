/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 21:08:20 by adeflers          #+#    #+#             */
/*   Updated: 2025/11/05 07:54:38 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (2);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (2);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (2);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (3);
	return (0);
}

int	exec_builtin(t_cmd cmd, t_line *line, int flag)
{
	if (ft_strncmp(cmd.cmd[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(cmd.cmd[0], "echo", 5) == 0)
		return (ft_echo(cmd));
	if (ft_strncmp(cmd.cmd[0], "env", 4) == 0)
		return (ft_env(line, cmd));
	if (ft_strncmp(cmd.cmd[0], "exit", 5) == 0)
		return (ft_exit(cmd, line, flag));
	if (ft_strncmp(cmd.cmd[0], "export", 7) == 0)
		return (ft_export(cmd, line));
	if (ft_strncmp(cmd.cmd[0], "unset", 6) == 0)
		return (ft_unset(cmd, line));
	if (ft_strncmp(cmd.cmd[0], "cd", 3) == 0)
		return (ft_cd(cmd, line));
	return (1);
}

int	is_option_n(char *s)
{
	int	i;

	if (!s)
		return (0);
	if (s[0] != '-')
		return (0);
	i = 1;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(t_cmd cmd)
{
	char	*str;
	int		i;
	int		n_flag;

	if (!cmd.cmd[1])
		return (write(STDOUT_FILENO, "\n", 1), 0);
	n_flag = 0;
	i = 1;
	while (is_option_n(cmd.cmd[i]) == 1)
	{
		n_flag = 1;
		i++;
	}
	str = cmd.cmd[i];
	while (cmd.cmd[i])
	{
		str = cmd.cmd[i];
		ft_putstr_fd(str, STDOUT_FILENO);
		i++;
		if (cmd.cmd[i])
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (n_flag == 0)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	ft_env(t_line *line, t_cmd cmd)
{
	int	i;

	if (cmd.cmd[1] != NULL)
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	i = 0;
	while (line->envp && line->envp[i])
	{
		ft_putstr_fd(line->envp[i], STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	return (0);
}
