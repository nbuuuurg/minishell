/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 21:08:20 by adeflers          #+#    #+#             */
/*   Updated: 2025/10/10 21:08:20 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	return (0);
} 

int	exec_builtin(t_cmd cmd, t_line *line)
{
	if (ft_strncmp(cmd.cmd[0], "cd", 3) == 0)
		return (ft_cd(cmd, line));
	if (ft_strncmp(cmd.cmd[0], "echo", 5) == 0)
		return (ft_echo(cmd, line));
	if (ft_strncmp(cmd.cmd[0], "env", 4) == 0)
		return (ft_env(cmd, line));
	if (ft_strncmp(cmd.cmd[0], "exit", 5) == 0)
		return (ft_exit(cmd, line));
	/* if (ft_strncmp(cmd.cmd[0], "export", 7) == 0) */
	/* 	return (ft_export(cmd, line)); */
	if (ft_strncmp(cmd.cmd[0], "pwd", 4) == 0)
		return (ft_pwd(cmd, line));
	if (ft_strncmp(cmd.cmd[0], "unset", 6) == 0)
		return (ft_unset(cmd, line));
	return (1);
}

int	ft_echo(t_cmd cmd, t_line *line)
{
	char	*str;

	if(ft_strncmp(cmd.cmd[1], "-n", 3) == 0)
		str = cmd.cmd[2];
	else
		str = cmd.cmd[1];
	if (ft_strncmp(str, "$?", 3) == 0) // provisoire
		str = ft_itoa(line->last_exit);
	ft_putstr_fd(str, STDOUT_FILENO);
	if (ft_strncmp(cmd.cmd[1], "-n", 3) != 0)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	ft_env(t_cmd cmd, t_line *line)
{
	int	i;

	(void)cmd;
	i = 0;
	while (line->envp[i])
	{
		ft_putstr_fd(line->envp[i], STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	return (0);
}

int	ft_export(t_cmd cmd, t_line *line) // ca marche pas
{
	char	*tmp;
	char	*str;
	int		i;

	i = 0;
	while (line->envp[i])
		i++;
	if (cmd.assign)
	{
		tmp = ft_strjoin(cmd.assign->name, "=");
		if (!tmp)
			return (perror("malloc"), 1);
		str = ft_strjoin(tmp, cmd.assign->value);
		if (!str)
			return (perror("malloc"), 1);
		free (tmp);
		line->envp = ft_realloc(line->envp, sizeof(char *) * i, sizeof(char *) * (i + 1));
		if (!line->envp)
			return (perror("malloc"), free(str), 1);
		line->envp[i] = ft_strdup(str);
		if (!line->envp[i])
			return (perror("malloc"), free(str), 1);
		line->envp[i + 1] = NULL;
		free(str);
	}
	else if (cmd.cmd[1] == NULL)
	{
		i = 0;
		while (line->envp[i])
		{
			printf("export ");
			printf("%s\n", line->envp[i]);
			i++;
		}
	}
	else
	{
		// faire comme le if d au dessus mais en rajoutant la ligne exported
	}
	return (0);
}

int	ft_unset(t_cmd cmd, t_line *line)
{
	(void)line;
	(void)cmd;
	return (0);
}

int	ft_pwd(t_cmd cmd, t_line *line)
{
	(void)line;
	(void)cmd;
	return (0);
}

int	ft_cd(t_cmd cmd, t_line *line)
{
	(void)line;
	(void)cmd;
	return (0);
}

int	ft_exit(t_cmd cmd, t_line *line)
{
	(void)line;
	(void)cmd;
	return (0);
}
