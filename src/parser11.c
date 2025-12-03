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

char	*parse__simple_expand(t_parse5 *p, t_token *token)
{
	if (token->s[p->i + 1] == '?')
	{
		p->var = ft_strdup("$?");
		p->end++;
	}
	else if (token->s[p->i + 1] == '$')
	{
		p->var = ft_strdup("$$");
		p->i++;
		p->end++;
	}
	if (!p->var)
		return (NULL);
	return (p->var);
}

void	define_len_and_i(t_token *token, t_parse5 *p)
{
	while (token->s[p->i + 1] && ft_isalnum(token->s[p->i + 1]))
	{
		p->len++;
		p->i++;
	}
}

char	*parse_expand_helper(t_parse5 *p, t_line *line, t_token *token)
{
	if (token->s[p->i + 1])
	{
		if (token->s[p->i + 1] == '?' || token->s[p->i + 1] == '$')
			p->var = parse__simple_expand(p, token);
		else
		{
			define_len_and_i(token, p);
			if (p->len == 0)
				p->var = ft_strdup("$");
			else
				p->var = ft_calloc(p->len + 1, 1);
			if (!p->var)
				return (NULL);
			if (p->len != 0)
				ft_memcpy(p->var, &token->s[p->i + 1 - p->len], p->len);
			p->end = p->i;
		}
	}
	else
		p->var = ft_strdup("$");
	if (!p->var)
		return (line->last_exit = EX_GEN, NULL);
	return (p->var);
}
