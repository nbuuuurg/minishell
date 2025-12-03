/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 21:00:26 by adeflers          #+#    #+#             */
/*   Updated: 2025/12/03 21:00:26 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	not_valid_identifier(char *s)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(s, STDERR_FILENO);
	ft_putstr_fd("\': not a valid identifier\n", STDERR_FILENO);
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

char	**unset_build_new_env(t_line *line, int exist_pos, char **new_env)
{
	int	src;
	int	dest;

	src = 0;
	dest = 0;
	while (line->envp && line->envp[src])
	{
		if (src == exist_pos)
			src++;
		else
		{
			new_env[dest] = ft_strdup(line->envp[src]);
			if (!new_env[dest])
			{
				free_split(new_env);
				return (NULL);
			}
			src++;
			dest++;
		}
	}
	new_env[dest] = NULL;
	return (new_env);
}

int	ft_unset(t_cmd cmd, t_line *line)
{
	char	**new_env;
	int		exist_pos;
	int		new_size;
	int		i;

	i = 1;
	while (cmd.cmd[i])
	{
		if (!cmd.cmd[i])
			return (0);
		exist_pos = var_exists(line, cmd.cmd[i]);
		if (exist_pos < 0)
			return (0);
		new_size = get_new_env_size(line, exist_pos, 0);
		new_env = malloc(sizeof(char *) * (new_size + 1));
		if (!new_env)
			return (perror("malloc"), 1);
		new_env = unset_build_new_env(line, exist_pos, new_env);
		if (!new_env)
			return (perror("malloc"), 1);
		if (replace_env(line, new_env) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("pwd: error retrieving current directory", STDERR_FILENO);
		ft_putstr_fd(": getcwd: cannot access parent", STDERR_FILENO);
		ft_putstr_fd(" directories: No such file or directory\n", STDERR_FILENO);
		return (1);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	free(cwd);
	return (0);
}
