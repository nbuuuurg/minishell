/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser13.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:33:41 by adeflers          #+#    #+#             */
/*   Updated: 2025/12/14 15:33:41 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*replace_var_1(int *len, char *var)
{
	*len = 1;
	var = ft_strdup("$");
	if (!var)
		return (perror("malloc"), NULL);
	return (var);
}

char	*replace_var_2(int *len, char *var, t_line *line)
{
	*len = 2;
	var = ft_strdup("$?");
	if (!var)
		return (perror("malloc"), NULL);
	var = expanded_var(line, var);
	if (!var)
		return (perror("malloc"), NULL);
	return (var);
}

char	*replace_var_3(int *len, char *var, t_line *line)
{
	*len = 2;
	var = ft_strdup("$$");
	if (!var)
		return (perror("malloc"), NULL);
	var = expanded_var(line, var);
	if (!var)
		return (perror("malloc"), NULL);
	return (var);
}

char	*replace_var_4(int *len, char *var)
{
	*len = 1;
	var = ft_strdup("$");
	if (!var)
		return (perror("malloc"), NULL);
	return (var);
}

char	*replace_var_5(char *s, int *len, char *var, t_line *line)
{
	int	i;
	int	j;

	i = 1;
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	if (i == 1)
		return (var = replace_var_4(len, var));
	var = malloc(sizeof(char) * i);
	if (!var)
		return (perror("malloc"), NULL);
	j = 0;
	while (j < i - 1)
	{
		var[j] = s[j + 1];
		j++;
	}
	var[j] = '\0';
	var = expanded_var(line, var);
	if (!var)
		return (perror("malloc"), NULL);
	*len = i;
	return (var);
}
