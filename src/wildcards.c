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

t_token	*lst_join(t_token *at, t_token *nw, t_line *line)
{
	t_token	*left;
	t_token	*right;
	t_token	*tail;

	(void)line;
	if (!at || !nw)
		return (at);
	left = at->previous;
	right = at->next;
	if (!left && !right)
	{
			free(at->s);
			free(at);
			return (line->tokens = nw); //ici probleme *
	}
	free(at->s);
	free(at);
	while (nw->previous)
		nw = nw->previous;
	if (left)
		left->next = nw;
	nw->previous = left;
	tail = nw;
	while (tail->next)
		tail = tail->next;
	tail->next = right;
	if (right)
		right->previous = tail;
	if (left)
		nw = left;
	while (nw->previous)
		nw = nw->previous;
	return (nw);
}

t_token	*last_elem_w(t_token *token)
{
	t_token	*temp;

	temp = NULL;
	if (token)
	{
		temp = token;
		while (temp->next)
			temp = temp->next;
	}
	return (temp);
}

t_token	*add_back_w(t_token *old, t_token *new)
{
	t_token	*temp;

	if (new)
	{
		if (!old)
		{
			old = new;
			new->next = NULL;
			new->previous = NULL;
		}
		else
		{
			temp = last_elem_w(old);
			temp->next = new;
			new->previous = temp;
			new->next = NULL;
		}
	}
	return (old);
}

int	match_tab(const char **pattern, char c)
{
	const char	*p;
	int			neg;
	int			ok;
	char		start;
	char		end;

	p = *pattern;
	neg = 0;
	ok = 0;
	if (*p == '!' || *p == '^')
	{
		neg = 1;
		p++;
	}
	while (*p && *p != ']')
	{
		if (*(p + 1) == '-' && *(p + 2) && *(p + 2) != ']')
		{
			start = *p;
			end = *(p + 2);
			if (start <= c && c <= end)
				ok = 1;
			p += 3;
		}
		else
		{
			if (*p == c)
				ok = 1;
			p++;
		}
	}
	if (*p == ']')
		p++;
	*pattern = p;
	if (neg != 0)
		return (1);
	else
		return (0);
}

int	ft_fnmatch(const char *pattern, const char *str)
{
	const char	*p;
	const char	*s;
	const char	*star_pat;
	const char	*star_str;

	p = pattern;
	s = str;
	star_pat = NULL;
	star_str = NULL;
	if (*s == '.' && *p != '.')
		return (0);
	while (*s)
	{
		if (*p == '*')
		{
			while (*p == '*')
				p++;
			star_pat = p;
			star_str = s;
		}
		else if (*p == '?')
		{
			if (*s == '/')
				return (0);
			p++;
			s++;
		}
		else if (*p == '[')
		{
			p++;
			if (*s == '/' || !match_tab(&p, *s))
				return (0);
			s++;
		}
		else if (*p == *s && *p != '\0')
		{
			p++;
			s++;
		}
		else if (star_pat)
		{
			star_str++;
			s = star_str;
			p = star_pat;
		}
		else
			return (0);
	}
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
				return (line->last_exit = 1, NULL);
			temp->s = ft_strdup(entry->d_name);
			if (!temp->s)
				return (free(temp), line->last_exit = 1, NULL);
			temp2 = add_back_w(temp2, temp);
		}
		entry = readdir(dir);
	}
	if (temp2)
		token = lst_join(token, temp2, line);
	closedir(dir);
	return (token);
}
