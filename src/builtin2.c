/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:40:35 by adeflers          #+#    #+#             */
/*   Updated: 2025/12/03 17:40:35 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	chdir_error_handling(char *path)
{
	ft_putstr_fd("cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	if (errno == ENOTDIR)
		ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
	else if (errno == EACCES)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
}

int	no_pwd(char *path, char *oldpwd, char *new_env_pwd, t_line *line)
{
	if (chdir(path) == -1)
		return (chdir_error_handling(path), 1);
	oldpwd = ft_strdup("OLDPWD= do not exist anymore");
	if (!oldpwd)
		return (perror("malloc"), 1);
	new_env_pwd = ft_strdup("PWD=..");
	if (!new_env_pwd)
		return (perror("malloc"), free(oldpwd), 1);
	update_env_cd(line, oldpwd, new_env_pwd);
	ft_putstr_fd("chdir: error retrieving current ", STDERR_FILENO);
	ft_putstr_fd("directory: getcwd: cannot access parent ", STDERR_FILENO);
	ft_putstr_fd("directories: No such file or directory\n", STDERR_FILENO);
	return (1);
}

int	ft_cd(t_cmd cmd, t_line *line)
{
	char	*path;
	int		i;

	i = 0;
	while (cmd.cmd[i] != NULL)
		i++;
	if (i > 2)
		return (ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO), 1);
	if (!cmd.cmd[1])
	{
		path = find_env_var(line, "HOME");
		if (!path)
			return (ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO), 1);
	}
	else
		path = cmd.cmd[1];
	if (ft_cd2(path, line) != 0)
		return (1);
	return (0);
}

int	ft_cd2(char *path, t_line *line)
{
	char	*temp;
	char	*oldpwd;
	char	*new_env_pwd;

	(1 && (oldpwd = NULL), (new_env_pwd = NULL));
	temp = getcwd(NULL, 0);
	if (!temp)
		return (no_pwd(path, oldpwd, new_env_pwd, line));
	oldpwd = ft_strjoin("OLDPWD=", temp);
	if (!oldpwd)
		return (perror("malloc"), 1);
	free(temp);
	if (chdir(path) == -1)
		return (chdir_error_handling(path), free(oldpwd), 1);
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
