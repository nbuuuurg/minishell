/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:59:20 by adeflers          #+#    #+#             */
/*   Updated: 2025/09/24 15:59:20 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* TODO :  */



/* - gerer les cas d'erreur --> comparer avec bash --posix */

// si mauvais chmode sur un fichier de redirection
// si cmd introuvable
// si pas de path
// si chmode sur un dossier ou executable


/* - gerer les builtins  */



/* - && --> executer chaque exprs  */
/* - 1 || 2 --> si exprs 1 marche ne pas exec exprs 2  */

char	**ft_strdup2(char **env)
{
	int		i;
	char	**new;

	i = 0;
	while (env[i])
		i++;
	new = malloc(sizeof(char *) * (i + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new[i] = ft_strdup(env[i]);
		if (!new[i])
			return (free_split(new), NULL);
		i++;
	}
	new[i] = NULL;
	return (new);
}
