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

char	*first_check_var(char *s, int *len, t_line *line)
{
	char	*var;

	*len = 0;
	var = NULL;
	if (s[1] == '\0')
	{
		*len = 1;
		var = ft_strdup("$");
	}
	else if (s[1] == '?')
	{
		*len = 2;
		var = ft_strdup("$?");
	}
	else if (s[1] == '$')
	{
		*len = 2;
		var = ft_strdup("$$");
	}
	if (!var)
		return (perror("malloc"), line->last_exit = 10, NULL);
	return (var);
}

char	*expanded_content_helper(char *s, char *new_s, t_line *line)
{
	t_parse6	p;

	p.i = 0;
	while (s[p.i])
	{
		p.len = 0;
		if (s[p.i] == '$')
		{
			p.temp = replace_var(&s[p.i], &p.len, line);
			if (!p.temp)
				return (free(new_s), NULL);
			p.i += p.len;
			p.tmp = ft_strjoin(new_s, p.temp);
			free(p.temp);
		}
		else
		{
			p.tmp = ft_strjoin_char(new_s, s[p.i]);
			p.i++;
		}
		if (!p.tmp)
			return (free(new_s), NULL);
		new_s = take_tmp_value(new_s, p.tmp);
	}
	return (new_s);
}

char	*expanded_content(char *s, t_line *line)
{
	char	*new_s;

	if (!s)
		return (NULL);
	new_s = ft_strdup("");
	if (!new_s)
		return (NULL);
	new_s = expanded_content_helper(s, new_s, line);
	if (!new_s)
		return (NULL);
	return (new_s);
}
