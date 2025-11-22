/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:04:11 by nburgevi          #+#    #+#             */
/*   Updated: 2025/08/25 18:16:23 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_token(t_line *line, char *s, int len)
{
	t_token	*token;

	(void)line;
	(void)s;
	if (len <= 0)
		return (NULL);
	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (line->last_exit = EX_GEN, NULL);
	token->s = ft_calloc(len + 1, 1);
	if (!token->s)
		return (free(token), line->last_exit = EX_GEN, NULL);
	ft_memcpy(token->s, s, len);
	token->s[len] = 0;
	token->next = NULL;
	token->quoted = NO_QUOTE;
	token->in_subshell = count_subshell(token->s);
	token->in_heredoc = 0;
	token = token_type(token);
	token = has_expand(token);
	if (has_wildcards(token->s))
		token->has_wildcards = 1;
	else
		token->has_wildcards = 0;
	token->w_expanded = 0;
	return (token);
}

t_token	*create_quoted_token(t_line *line, char *s, int len, int quote)
{
	t_token	*token;
	int		i;
	int		multiple_quote;

	(void)line;
	(void)s;
	if (len <= 0)
		return (NULL);
	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (line->last_exit = EX_GEN, NULL);
	token->s = ft_calloc(len + 1, 1);
	if (!token->s)
		return (free(token), line->last_exit = EX_GEN, NULL);
	multiple_quote = 0;
	i = -1;
	while (++i < len)
	{
		token->s[i] = s[i];
		if (is_quote(token->s[i]))
			multiple_quote++;
	}
	init_token(token, multiple_quote, quote, i);
	token->has_wildcards = 0;
	token->w_expanded = 0;
	return (token);
}

t_token	*last_elem(t_line *line)
{
	t_token	*temp;

	temp = NULL;
	if (line->tokens)
	{
		temp = line->tokens;
		while (temp->next)
			temp = temp->next;
	}
	return (temp);
}

void	add_back(t_line *line, t_token *new)
{
	t_token	*temp;

	if (new)
	{
		if (!line->tokens)
		{
			line->tokens = new;
			new->next = NULL;
			new->previous = NULL;
		}
		else
		{
			temp = last_elem(line);
			temp->next = new;
			new->previous = temp;
			new->next = NULL;
		}
	}
	line->last_exit = EX_OK;
	return ;
}
