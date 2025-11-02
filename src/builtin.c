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
	// builtin qui n ont pas d effet sur l env 
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);

	// builtin qui ont un effet sur l env

	if (ft_strncmp(cmd, "export", 7) == 0)
		return (2);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (2);
	/* if (ft_strncmp(cmd, "cd", 3) == 0) */
	/* 	return (2); */

	// exit 

	/* if (ft_strncmp(cmd, "exit", 5) == 0) */
	/* 	return (3); */
	return (0);
} 

int	exec_builtin(t_cmd cmd, t_line *line)
{
	if (ft_strncmp(cmd.cmd[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(cmd.cmd[0], "echo", 5) == 0)
		return (ft_echo(cmd, line));
	if (ft_strncmp(cmd.cmd[0], "env", 4) == 0)
		return (ft_env(line));
	/* if (ft_strncmp(cmd.cmd[0], "exit", 5) == 0) */
	/* 	return (ft_exit(cmd, line)); */
	if (ft_strncmp(cmd.cmd[0], "export", 7) == 0)
		return (ft_export(cmd, line));
	if (ft_strncmp(cmd.cmd[0], "unset", 6) == 0)
		return (ft_unset(cmd, line));
	/* if (ft_strncmp(cmd.cmd[0], "cd", 3) == 0) */
	/* 	return (ft_cd(cmd, line)); */
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

int ft_echo(t_cmd cmd, t_line *line)
{
    char *str;
    int   i;
    int   n_flag;
    int   need_free;

    if (!cmd.cmd[1])
        return (write(STDOUT_FILENO, "\n", 1), 0);
    n_flag = 0;
    i = 1;
    while (is_option_n(cmd.cmd[i]) == 1)
	{
		n_flag = 1; 
		i++; 
	}
    need_free = 0;
	str = cmd.cmd[i];
	while(cmd.cmd[i])
	{
		str = cmd.cmd[i];
		if (ft_strncmp(str, "$?", 3) == 0)
		{
			str = ft_itoa(line->prev_exit);
			if (!str) 
				return (perror("malloc"), 1);
			need_free = 1;
		}
		ft_putstr_fd(str, STDOUT_FILENO);
		i++;
		if (cmd.cmd[i])
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
    if (n_flag == 0)
        write(STDOUT_FILENO, "\n", 1);
    if (need_free)
        free(str);
    return (0);
}

int	ft_env(t_line *line)
{
	int	i;

	i = 0;
	while (line->envp && line->envp[i])
	{
		ft_putstr_fd(line->envp[i], STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	return (0);
}

int	ft_pwd(void)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
    if (!cwd)
        return (perror("getcwd"), 1);
    ft_putstr_fd(cwd, STDOUT_FILENO);
    write(STDOUT_FILENO, "\n", 1);
    free(cwd);
    return (0);
}

int	var_exists(t_line *line, char *name)
{
	int		i;
	int		j;
	
	i = 0;
	while (name[i] && name[i] != '=')
		i++;
	j = 0;
	while (line->envp && line->envp[j])
	{
		if (ft_strncmp(line->envp[j], name, i) == 0)
			if (line->envp[j][i] == '=')
				return (j);
		j++;
	}
	return (-1);
}

int	ft_export(t_cmd cmd, t_line *line)
{
	char	**new_env;
	int		old_size;
	int		new_size;
	int		exist_pos;
	int		src;
	int		dest;

	if (!cmd.cmd[1] || is_assignment(cmd.cmd[1]) == 0)
		return (1);
	exist_pos = var_exists(line, cmd.cmd[1]);
	old_size = 0;
	while (line->envp && line->envp[old_size])
		old_size++;
	if (exist_pos >= 0)
		new_size = old_size;
	else
		new_size = old_size + 1;
	new_env = malloc(sizeof(char *) * (new_size + 1));
	if (!new_env)
		return (perror("malloc"), 1);
	src = 0;
	dest = 0;
	while (src < old_size)
	{
		if (src == exist_pos)
		{
			new_env[dest] = ft_strdup(cmd.cmd[1]);
			if (!new_env[dest])
				return (perror("malloc"), free_split(new_env), 1);
		}
		else
		{
			new_env[dest] = ft_strdup(line->envp[src]);
			if (!new_env[dest])
				return (perror("malloc"), free_split(new_env), 1);
		}
		src++;
		dest++;
	}
	if (exist_pos < 0)
	{
		new_env[dest] = ft_strdup(cmd.cmd[1]);
		if (!new_env[dest])
			return (perror("malloc"), free_split(new_env), 1);
		dest++;
	}
	new_env[dest] = NULL;
	if (line->envp)
		free_split(line->envp);
	line->envp = ft_strdup2(new_env);
	if (!line->envp)
		return (perror("malloc"), free_split(new_env), 1);
	free_split(new_env);
	return (0);
}

int	ft_unset(t_cmd cmd, t_line *line)
{
	char	**new_env;
	int		exist_pos;
	int		old_size;
	int		new_size;
	int		src;
	int		dest;

	if (!cmd.cmd[1])
		return (1);
	old_size = 0;
	while (line->envp && line->envp[old_size])
		old_size++;
	exist_pos = var_exists(line, cmd.cmd[1]);
	if (exist_pos < 0)
		return (0);
	new_size = old_size - 1;
	new_env = malloc(sizeof(char *) * (new_size + 1));
	if (!new_env)
		return (perror("malloc"), 1);
	src = 0;
	dest = 0;
	while (src < old_size)
	{
		if (src == exist_pos)
			src++;
		else
		{
			new_env[dest] = ft_strdup(line->envp[src]);
			if (!new_env[dest])
				return (perror("malloc"), free_split(new_env), 1);
			src++;
			dest++;
		}
	}
	new_env[dest] = NULL;
	if (line->envp)
		free_split(line->envp);
	line->envp = ft_strdup2(new_env);
	if (!line->envp)
		return (perror("malloc"), free_split(new_env), 1);
	free_split(new_env);
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
