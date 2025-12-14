/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 08:56:49 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:56:51 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	end_of_var(int *i, char *s, char *var)
{
	int	j;

	j = 0;
	while (j < *i - 1)
	{
		var[j] = s[j + 1];
		j++;
	}
	return (j);
}

char	*find_expanded_var(int i, char *s, t_line *line)
{
	char	*var;
	int		j;

	var = malloc(sizeof(char) * (i));
	if (!var)
		return (perror("malloc"), line->last_exit = 10, NULL);
	j = end_of_var(&i, s, var);
	var[j] = '\0';
	var = expanded_var(line, var);
	if (!var)
		return (perror("malloc"), NULL);
	return (var);
}

char	*replace_var(char *s, int *len, t_line *line)
{
	char	*var;

	var = NULL;
	if (s[1] == '\0')
		return (var = replace_var_1(len, var));
	if (s[1] == '?')
		return (var = replace_var_2(len, var, line));
	if (s[1] == '$')
		return (var = replace_var_3(len, var, line));
	var = replace_var_5(s, len, var, line);
	return (var);
}

char	*ft_strjoin_char(char *s, char c)
{
	char	*new_s;
	int		i;

	if (!s)
		return (NULL);
	new_s = malloc(sizeof(char) * (ft_strlen(s) + 2));
	if (!new_s)
		return (perror("malloc"), NULL);
	i = 0;
	while (s[i])
	{
		new_s[i] = s[i];
		i++;
	}
	new_s[i++] = c;
	new_s[i] = '\0';
	return (new_s);
}

char	*take_tmp_value(char *new_s, char *tmp)
{
	free(new_s);
	return (tmp);
}
