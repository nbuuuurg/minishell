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
		return (2);
	return (0);
} 

int	exec_builtin(t_cmd cmd, t_line *line)
{
	if (ft_strncmp(cmd.cmd[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(cmd.cmd[0], "echo", 5) == 0)
		return (ft_echo(cmd, line));
	if (ft_strncmp(cmd.cmd[0], "env", 4) == 0)
		return (ft_env(line, cmd));
	if (ft_strncmp(cmd.cmd[0], "exit", 5) == 0)
		return (ft_exit(cmd, line));
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

int	ft_pwd(void)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("pwd: error retrieving current directory: getcwd: cannot access parent directories"), 1);
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
int    ft_export(t_cmd cmd, t_line *line)
{
    char    **new_env;
    int        old_size;
    int        new_size;
    int        exist_pos;
    int        src;
    int        dest;
    int        i;

    i = 1;
    while (cmd.cmd[i])
    {
        if (has_equal(cmd.cmd[i]) == 0)
            return (0);
        if (is_assignment(cmd.cmd[i]) == 0)
        {
            ft_putstr_fd("export: `", STDERR_FILENO);
            ft_putstr_fd(cmd.cmd[i], STDERR_FILENO);
            ft_putstr_fd("\': not a valid identifier\n", STDERR_FILENO);
            return (1);
        }
        exist_pos = var_exists(line, cmd.cmd[i]);
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
                new_env[dest] = ft_strdup(cmd.cmd[i]);
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
            new_env[dest] = ft_strdup(cmd.cmd[i]);
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
        i++;
    }
    return (0);
}

int    ft_unset(t_cmd cmd, t_line *line)
{
    char    **new_env;
    int        exist_pos;
    int        old_size;
    int        new_size;
    int        src;
    int        dest;
    int        i;

    i = 1;
    while (cmd.cmd[i])
    {
        if (!cmd.cmd[i])
            return (0);
        old_size = 0;
        while (line->envp && line->envp[old_size])
            old_size++;
        exist_pos = var_exists(line, cmd.cmd[i]);
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
        i++;
    }
    return (0);
}

void	update_env_cd(t_line *line, char *oldpwd, char *newpwd)
{
	int		i;

	i = 0;
	while (line->envp && line->envp[i])
	{
		if (ft_strncmp(line->envp[i], "OLDPWD=", 7) == 0)
		{
			free(line->envp[i]);
			line->envp[i] = oldpwd;
		}
		else if (ft_strncmp(line->envp[i], "PWD=", 4) == 0)
		{
			free(line->envp[i]);
			line->envp[i] = newpwd;
		}
		i++;
	}
}

int	cd_too_many_args(t_cmd cmd)
{
	int	i;

	i = 0;
	while (cmd.cmd[i] != NULL)
		i++;
	return (i);
}

int	ft_cd(t_cmd cmd, t_line *line)
{
	char	*path;
	char	*oldpwd;
	char	*new_env_pwd;
	char	*temp;

	// a fix le cas de merde qd on suprime le dossier dans lequel on est 

	if (cd_too_many_args(cmd) > 2)
		return (ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO), 1);
	if (!cmd.cmd[1])
	{
		path = find_env_var(line, "HOME");
		if (!path)
			return (ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO), 1);
	}
	else
		path = cmd.cmd[1];
	temp = getcwd(NULL, 0);
	if (!temp)
	{
		if (chdir(path) == -1)
		{
			ft_putstr_fd("cd: ", STDERR_FILENO);
			ft_putstr_fd(path, STDERR_FILENO);
			if (errno == ENOTDIR)
				ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
			else if (errno == EACCES)
				ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			else
				ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			return (1);
		}
		oldpwd = ft_strdup("OLDPWD= do not exist anymore");
		if (!oldpwd)
			return (perror("malloc"), 1);
		new_env_pwd = ft_strdup("PWD=..");
		if (!new_env_pwd)
			return (perror("malloc"), free(oldpwd), 1);
		update_env_cd(line, oldpwd, new_env_pwd);
		return (perror("chdir: error retrieving current directory: getcwd: cannot access parent directories"), 1);
	}
	oldpwd = ft_strjoin("OLDPWD=", temp);
	if (!oldpwd)
		return (perror("malloc"), 1);
		// perror("malloc"); // on vire le return pour qd meme arriver au chdir pour le cas de merde
	free(temp);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		if (errno == ENOTDIR)
			ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
		else if (errno == EACCES)
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		else
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (free(oldpwd), 1);
	}
	temp = getcwd(NULL, 0);
	if (!temp)
		return (perror("getcwd"), free(oldpwd), 1);
	new_env_pwd = ft_strjoin("PWD=", temp);
	if (!new_env_pwd)
		return (perror("malloc"), free(oldpwd), free(temp), 1);
	free(temp);
	update_env_cd(line, oldpwd, new_env_pwd);
	return (0);
}

long	ft_atol(char *s)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
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
	// attention long long min
	return ((result * sign) % 256);
}

int	ft_exit(t_cmd cmd, t_line *line)
{
	long	exit_code;
	int		last_exit;

	if (!cmd.cmd[1])
	{
		last_exit = line->prev_exit;
		write(STDOUT_FILENO, "exit\n", 5);
		free_exec_cmd(line);
		clear_history();
		exit(last_exit);
	}
	else if (ft_isdigit_str(cmd.cmd[1]) == 0)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO);
		free_exec_cmd(line);
		clear_history();
		exit(2);
	}
	else if (cmd.cmd[1] && cmd.cmd[2])
	{
		write(STDOUT_FILENO, "exit\n", 5);
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else if (cmd.cmd[1])
	{
		exit_code = ft_atol(cmd.cmd[1]);
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
	return (0);
}
