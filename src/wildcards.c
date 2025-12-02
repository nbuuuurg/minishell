/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 01:36:29 by adeflers          #+#    #+#             */
/*   Updated: 2025/11/05 09:10:17 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_fnmatch_loop(char **s, char **p, char **star_pat, char **star_str)
{
	while (**s)
	{
		if (**p == '*')
		{
			while (**p == '*')
				(*p)++;
			*star_pat = *p;
			*star_str = *s;
		}
		else if (**p == '?' && **s == '/')
			return (0);
		else if ((**p == '?' || **p == **s) && **p != '\0')
		{
			(*p)++;
			(*s)++;
		}
		else if (*star_pat)
		{
			*s = ++(*star_str);
			*p = *star_pat;
		}
		else
			return (0);
	}
	return (1);
}

int	ft_fnmatch(char *pattern, char *str)
{
	char	*p;
	char	*s;
	char	*star_pat;
	char	*star_str;

	p = pattern;
	s = str;
	star_pat = NULL;
	star_str = NULL;
	if (*s == '.' && *p != '.')
		return (0);
	if (!ft_fnmatch_loop(&s, &p, &star_pat, &star_str))
		return (0);
	while (*p == '*')
		p++;
	return (*p == '\0');
}

t_token	*parse_wildcards(t_line *line, t_token *token)
{
	t_token			*temp2;
	t_token			*temp;
	DIR				*dir;
	struct dirent	*entry;

	temp2 = NULL;
	dir = opendir(".");
	if (!dir)
		return (line->last_exit = 1, NULL);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (ft_fnmatch(token->s, entry->d_name) == 1)
		{
			temp = ft_calloc(1, sizeof(t_token));
			if (!temp)
				return (line->last_exit = 1, closedir(dir), NULL);
			temp->s = ft_strdup(entry->d_name);
			if (!temp->s)
				return (free(temp), line->last_exit = 1, closedir(dir), NULL);
			temp2 = add_back_w(temp2, temp);
		}
		entry = readdir(dir);
	}
	return (closedir(dir), token = lst_join(token, temp2, line), token);
}
