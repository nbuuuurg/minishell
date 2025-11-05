/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 01:43:49 by adeflers          #+#    #+#             */
/*   Updated: 2025/11/05 08:09:35 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*lexer_special_char4(t_line *line, char **s, char **start, char **end)
{
	*start = *s;
	*end = *start;
	while (**end && !is_whitespace(**end))
	{
		if (**end == '>' || **end == '<')
		{
			if (*(*end + 1) && *(*end + 1) == **end)
			{
				add_back(line, create_token(line, *end, 2));
				(*end)++;
			}
			else
				add_back(line, create_token(line, *end, 1));
			if (line->last_exit != 0)
				return (NULL);
			return (*s = *end);
		}
		if (**end == '|')
		{
			if (*(*end + 1) && *(*end + 1) == **end)
			{
				add_back(line, create_token(line, *end, 2));
				(*end)++;
			}
			else
				add_back(line, create_token(line, *end, 1));
			if (line->last_exit != 0)
				return (NULL);
			return (*s = *end);
		}
		(*end)++;
	}
	add_back(line, create_token(line, *start, (*end - *start) + 1));
	if (line->last_exit != 0)
		return (NULL);
	return (*s = *end);
}

char	*lexer_special_char(t_line *line, char *s, char *start, char *end)
{
	if (s == line->input)
		return (s = lexer_special_char4(line, &s, &start, &end));
	if (*(s - 1) && !is_whitespace(*(s - 1)))
	{
		end = s - 1;
		add_back(line, create_token(line, start, (end - start) + 1));
		if (line->last_exit != 0)
			return (NULL);
	}
	if (*(s + 1))
	{
		if (*(s + 1) == is_special(*s))
			s = lexer_special_char2(line, &s, &start, &end);
		else
			s = lexer_special_char3(line, &s, &start, &end);
	}
	return (s);
}

char	*lexer_special_char2(t_line *line, char **s, char **start, char **end)
{
	*start = *s;
	*end = *s + 1;
	add_back(line, create_token(line, *start, (*end - *start) + 1));
	if (line->last_exit != 0)
		return (NULL);
	if (*(*s + 2) == 0)
		return (*s + 2);
	return (++(*s));
}

char	*lexer_special_char3(t_line *line, char **s, char **start, char **end)
{
	*start = *s;
	*end = *s;
	add_back(line, create_token(line, *start, (*end - *start) + 1));
	if (line->last_exit != 0)
		return (NULL);
	return (*s);
}
