/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 18:55:29 by nburgevi          #+#    #+#             */
/*   Updated: 2025/08/24 18:37:04 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**get_path(char **env)
{
	char	**path;
	char	*all_path = NULL;
	char	**tmp;
	int		i;
	int		k;

	if (!env)
		return (NULL); // error pas d'env
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
			all_path = env[i] + 5 ;
	}
	if (!all_path)
		return (NULL); // error pas de PATH

	tmp = ft_split(all_path, ':');
	if (!tmp)
		return (perror("ft_split"), NULL);
	i = 0;
	while (tmp[i])
		i++;
	path = malloc(sizeof(char *) * (i + 1));
	if (!path)
		return (NULL); // error malloc
	i = -1;
	while (tmp[++i])
	{
		path[i] = ft_strjoin(tmp[i], "/");
		if (!path[i])
		{
		    // free partiel si échec au milieu
		    k = 0;
		    while (k < i)
		        free(path[k++]);
		    free(path);
		    free_split(tmp);
		    return (NULL);
		}
		// if (!path[i])
		// 	return (free_split(tmp), NULL); // error malloc
	}
	path[i] = NULL;
	return (free_split(tmp), path);
}