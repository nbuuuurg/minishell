/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:00:24 by adeflers          #+#    #+#             */
/*   Updated: 2025/12/03 19:00:24 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	copy_env_var(char **new_env, char *new_var, int dest)
{
	new_env[dest] = ft_strdup(new_var);
	if (!new_env[dest])
		return (free_split(new_env), -1);
	return (0);
}

char	**export_new_env(t_line *line, int new_size,
					int exist_pos, char *new_var)
{
	char	**new_env;
	int		src;
	int		dest;

	(1 && (src = 0), (dest = 0));
	new_env = malloc(sizeof(char *) * (new_size + 1));
	if (!new_env)
		return (NULL);
	while (line->envp && line->envp[src])
	{
		if (src == exist_pos)
		{
			if (copy_env_var(new_env, new_var, dest) != 0)
				return (NULL);
		}
		else
			if (copy_env_var(new_env, line->envp[src], dest) != 0)
				return (NULL);
		src++;
		dest++;
	}
	if (exist_pos < 0)
		if (copy_env_var(new_env, new_var, dest++) != 0)
			return (NULL);
	return (new_env[dest] = NULL, new_env);
}

int	get_new_env_size(t_line *line, int exist_pos, int flag)
{
	int	old_size;
	int	new_size;

	old_size = 0;
	while (line->envp && line->envp[old_size])
		old_size++;
	if (flag)
	{
		if (exist_pos >= 0)
			new_size = old_size;
		else
			new_size = old_size + 1;
		return (new_size);
	}
	else
	{
		new_size = old_size - 1;
		return (new_size);
	}
}

int	replace_env(t_line *line, char **new_env)
{
	if (line->envp)
		free_split(line->envp);
	line->envp = ft_strdup2(new_env);
	if (!line->envp)
		return (free_split(new_env), 1);
	free_split(new_env);
	return (0);
}

int	ft_export(t_cmd cmd, t_line *line)
{
	char	**new_env;
	int		new_size;
	int		exist_pos;
	int		i;
	int		ex_code;

	(1 && (i = 0), (ex_code = 0));
	while (cmd.cmd[++i])
	{
		if (has_equal(cmd.cmd[i]) != 0)
		{
			if (is_assignment(cmd.cmd[i]) == 0)
				not_valid_identifier(cmd.cmd[i], &ex_code);
			else
			{
				exist_pos = var_exists(line, cmd.cmd[i]);
				new_size = get_new_env_size(line, exist_pos, 1);
				new_env = export_new_env(line, new_size, exist_pos, cmd.cmd[i]);
				if (!new_env || replace_env(line, new_env) != 0)
					return (perror("malloc"), 1);
			}
		}
	}
	return (ex_code);
}
