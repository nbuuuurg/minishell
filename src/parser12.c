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
	int		i;

	var = first_check_var(s, len, line);
	if (line->last_exit == 10)
		return (perror("malloc"), NULL);
	i = 1;
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	if (i == 1)
	{
		*len = 1;
		var = ft_strdup("$");
		if (!var)
			return (perror("malloc"), NULL);
		return (var);
	}
	else
	{
		var = find_expanded_var(i, s, line);
		if (!var)
			return (NULL);
	}
	*len = i;
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
