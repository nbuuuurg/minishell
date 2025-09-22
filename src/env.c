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

char	**get_path(void)
{
	char	**path;
	char	*all_path;

	all_path = getenv("PATH");
	if (!all_path)
		return (NULL);
	path = ft_split(all_path, ':');
	if (!path)
		exit((perror("ft_split"), EX_GEN));
	return (path);
}
