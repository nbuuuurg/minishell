/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:31 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:19:54 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	expand_and_quote_filename(t_line *line, t_token *temp)
{
	if (temp->has_expand != 0)
	{
		temp->s = parse_expand(line, temp);
		if (line->last_exit == EX_GEN)
			return (EX_GEN);
	}
	if (temp->quoted != NO_QUOTE)
		temp->s = parse_quoted_token(line, temp);
	if (line->last_exit == EX_GEN)
		return (EX_GEN);
	return (0);
}

int	set_redirect_file(t_line *line, t_expr1 *exp,
							t_token *temp, t_parse2 *p)
{
	if (temp->next && temp->next->type == WORD
		&& temp->next->in_subshell == 0)
	{
		if (expand_and_quote_filename(line, temp->next) == EX_GEN)
			return (EX_GEN);
		exp->new->pipeline[p->i].redirect[*p->j].file
			= ft_strdup(temp->next->s);
	}
	else
	{
		exp->new->pipeline[p->i].redirect[*p->j].file = NULL;
		line->lexer_err = -5;
	}
	return (0);
}

int	need_split_expand(t_token *temp)
{
	if ((sft_count_words(temp->s, ' ') > 1
			|| sft_count_words(temp->s, '\t') > 1)
		&& temp->has_expand > 0 && temp->quoted == NO_QUOTE)
		return (1);
	return (0);
}
