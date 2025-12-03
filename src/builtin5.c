/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 21:38:27 by adeflers          #+#    #+#             */
/*   Updated: 2025/12/03 21:38:27 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

long	ft_atol(char *s)
{
	long long	result;
	int			sign;
	int			i;

	(1 && (result = 0), (sign = 1), (i = 0));
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	while (s[i])
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result = result * 10 + (s[i] - '0');
			i++;
		}
		else
			return (555);
	}
	if (result > 9223372036854775807 || (result * sign) < -9223372036854775807)
		return (555);
	return ((result * sign) % 256);
}

void	exit_no_arg(t_line *line, int flag)
{
	int	last_exit;

	last_exit = line->prev_exit;
	if (flag)
		write(STDOUT_FILENO, "exit\n", 5);
	free_exec_cmd(line);
	clear_history();
	exit(last_exit);
}

void	exit_numric_arg_required(t_cmd cmd, int flag, t_line *line)
{
	if (flag)
		write(STDOUT_FILENO, "exit\n", 5);
	ft_putstr_fd("exit: ", STDERR_FILENO);
	ft_putstr_fd(cmd.cmd[1], STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	free_exec_cmd(line);
	clear_history();
	exit(2);
}

void	exit_with_choosen_code(t_cmd cmd, t_line *line, int flag)
{
	long	exit_code;

	exit_code = ft_atol(cmd.cmd[1]);
	if (flag)
		write(STDOUT_FILENO, "exit\n", 5);
	free_exec_cmd(line);
	clear_history();
	if (exit_code == 555)
	{
		ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
	else
		exit(exit_code);
}

int	ft_exit(t_cmd cmd, t_line *line, int flag)
{
	if (!cmd.cmd[1])
		exit_no_arg(line, flag);
	else if (ft_isdigit_str(cmd.cmd[1]) == 0)
		exit_numric_arg_required(cmd, flag, line);
	else if (cmd.cmd[1] && cmd.cmd[2])
	{
		if (flag)
			write(STDOUT_FILENO, "exit\n", 5);
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else if (cmd.cmd[1])
		exit_with_choosen_code(cmd, line, flag);
	return (0);
}
