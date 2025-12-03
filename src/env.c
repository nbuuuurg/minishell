/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 18:55:29 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 07:52:25 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*find_all_path(char **env)
{
	char	*all_path;
	int		i;

	if (!env)
		return (NULL);
	all_path = NULL;
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
			all_path = env[i] + 5 ;
	}
	if (!all_path)
		return (NULL);
	return (all_path);
}

char	**get_path(char **env)
{
	char	**path;
	char	*all_path;
	char	**tmp;
	int		i;
	int		k;

	all_path = find_all_path(env);
	if (!all_path)
		return (NULL);
	tmp = ft_split(all_path, ':');
	if (!tmp)
		return (perror("ft_split"), NULL);
	i = 0;
	while (tmp[i])
		i++;
	path = malloc(sizeof(char *) * (i + 1));
	if (!path)
		return (NULL);
	i = -1;
	while (tmp[++i])
	{
		path[i] = ft_strjoin(tmp[i], "/");
		if (!path[i])
		{
			k = 0;
			while (k < i)
				free(path[k++]);
			free(path);
			free_split(tmp);
			return (NULL);
		}
	}
	path[i] = NULL;
	return (free_split(tmp), path);
}
