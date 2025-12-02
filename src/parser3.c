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

char	*replace_var(char *s, int *len, t_line *line)
{
	char	*var;
	int		i;
	int		j;

	if (s[1] == '\0')
	{
		*len = 1;
		var = ft_strdup("$");
		if (!var)
			return (perror("malloc"), NULL);
		return (var);
	}
	if (s[1] == '?')
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
	if (s[1] == '$')
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

char	*expanded_content(char *s, t_line *line)
{
	char	*new_s;
	int		i;
	int		len;
	char	*temp;
	char	*tmp;

	if (!s)
		return (NULL);
	new_s = ft_strdup("");
	if (!new_s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		len = 0;
		if (s[i] == '$')
		{
			temp = replace_var(&s[i], &len, line);
			if (!temp)
				return (free(new_s), NULL);
			i += len;
			tmp = ft_strjoin(new_s, temp);
			free(temp);
			if (!tmp)
				return (free(new_s), NULL);
		}
		else
		{
			tmp = ft_strjoin_char(new_s, s[i]);
			if (!tmp)
				return (free(new_s), NULL);
			i++;
		}
		free(new_s);
		new_s = tmp;
	}
	return (new_s);
}
