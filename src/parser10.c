/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:31 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:25:26 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*parse_multiple_quotes(t_line *line, t_token *token, t_parse4 *p)
{
	p->len = find_token_length(token, p);
	p->s = ft_calloc(ft_strlen(token->s) - p->len + 1, 1);
	if (!p->s)
		return (line->last_exit = EX_GEN, NULL);
	init_parse4(p);
	while (token->s[p->i])
	{
		if (token->s[p->i] == 34 && p->s_quote == 0)
		{
			p->d_quote++;
			if (p->d_quote == 2)
				p->d_quote = 0;
		}
		else if (token->s[p->i] == 39 && p->d_quote == 0)
		{
			p->s_quote++;
			if (p->s_quote == 2)
				p->s_quote = 0;
		}
		else
			p->s[p->j++] = token->s[p->i];
		p->i++;
	}
	return (p->s);
}

char	*parse_quoted_token(t_line *line, t_token *token)
{
	t_parse4	p;

	init_parse4(&p);
	if (!token)
		return (line->last_exit = EX_GEN, NULL);
	if (token->quoted == SINGLE || token->quoted == DOUBLE)
	{
		p.s = ft_calloc(ft_strlen(token->s) - 1, 1);
		if (!p.s)
			return (line->last_exit = EX_GEN, NULL);
		while (token->s[p.i])
		{
			if (is_quote(token->s[p.i]))
				p.i++;
			else if (token->s[p.i])
				p.s[p.j++] = token->s[p.i++];
		}
	}
	else
	{
		p.s = parse_multiple_quotes(line, token, &p);
		if (!p.s)
			return (line->last_exit = EX_GEN, NULL);
	}
	return (free(token->s), p.s);
}

void	init_parse5(t_parse5 *p)
{
	p->i = 0;
	p->start = 0;
	p->end = 0;
	p->len = 0;
	p->s_quote = 0;
	p->d_quote = 0;
	p->var = NULL;
}

void	init_parse5_loop(t_parse5 *p)
{
	p->start = p->i;
	p->end = p->i;
	p->len = 0;
	p->var = NULL;
}

void	define_quotes(t_parse5 *p, t_token *token)
{
	if (token->s[p->i] == 34)
	{
		if (p->d_quote == 0 && p->s_quote == 0)
			p->d_quote = 1;
		else if (p->d_quote == 1)
			p->d_quote = 0;
	}
	else if (token->s[p->i] == 39)
	{
		if (p->s_quote == 0 && p->d_quote == 0)
			p->s_quote = 1;
		else if (p->s_quote == 1)
			p->s_quote = 0;
	}
}
