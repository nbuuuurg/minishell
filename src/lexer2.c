/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:24 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:05:27 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*lexer_input_something(t_line *line, char *s, char *start, char *end)
{
	if (*s && is_special(*s))
		s = lexer_special_char(line, s, start, end);
	else if (*s && is_whitespace(*s))
		s = lexer_simple_char(line, s, start, end);
	else if (*s && is_quote(*s))
		s = lexer_quoted_char(line, s, start, end);
	else if (*s && is_subshell(*s))
		s = lexer_subchell_char(line, s, start, end);
	if (line->last_exit != 0)
		return (NULL);
	return (s);
}

char	*lexer_simple_char(t_line *line, char *s, char *start, char *end)
{
	if (line->len != 1 && *(s - 1) && !is_special(*(s - 1)))
	{
		end = s - 1;
		add_back(line, create_token(line, start, (end - start) + 1));
		if (line->last_exit != 0)
			return (NULL);
	}
	return (s);
}

char	*lexer_skip_quoted(t_line *line, char *s, int *quote)
{
	*quote = is_quote(*s);
	s++;
	while (*s && *s != *quote)
		s++;
	if (*s == 0)
		line->lexer_err = -1;
	return (s);
}

char	*lexer_quoted_char(t_line *line, char *s, char *start, char *end)
{
	int	quote;

	s = lexer_skip_quoted(line, s, &quote);
	while (*s && !is_whitespace(*s) && !is_special(*s))
	{
		s++;
		if (*s && is_quote(*s))
			s = lexer_skip_quoted(line, s, &quote);
	}
	end = s - 1;
	add_back(line, create_quoted_token(line, start, (end - start) + 1, quote));
	if (line->last_exit != 0)
		return (NULL);
	return (s);
}

char	*lexer_subchell_char(t_line *line, char *s, char *start, char *end)
{
	int	open;
	int	close;
	int	temp_last_exit;

	open = 1;
	close = 0;
	temp_last_exit = 0;
	while (*s && open != close)
	{
		s++;
		if (*s == '(')
			open++;
		if (*s == ')')
			close++;
	}
	end = s;
	add_back(line, create_token(line, start, (end - start) + 1));
	if (line->last_exit != 0)
		return (NULL);
	return (line->lexer_err = temp_last_exit, s);
}
