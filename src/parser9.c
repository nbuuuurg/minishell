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

void	count_token(t_token *temp, int (*len)[3], t_token_type op_ctrl)
{
	t_token	*temp2;

	temp2 = temp;
	while (temp2 && temp2->type != PIPE && temp2->type != op_ctrl)
	{
		if (temp2->type == WORD)
			(*len)[0]++;
		if (temp2->type == REDIR_IN || temp2->type == REDIR_APPEND
			|| temp2->type == REDIR_OUT || temp2->type == HEREDOC)
			(*len)[1]++;
		if (temp2->type == ASSIGNMENT)
			(*len)[2]++;
		temp2 = temp2->next;
	}
}

void	find_pipe_position(t_expr *new, t_token *temp, int i)
{
	if (temp)
	{
		if (new->pipe_count == 0)
			new->pipeline[i].position = NO_PIPE;
		if (new->pipe_count != 0)
		{
			if (i == 0)
				new->pipeline[i].position = FIRST;
			else if (i != 0 && i < new->pipe_count)
				new->pipeline[i].position = MIDDLE;
			else
				new->pipeline[i].position = LAST;
		}
	}
	else if (!temp)
	{
		if (new->pipe_count == 0)
			new->pipeline[i].position = NO_PIPE;
		else
			new->pipeline[i].position = LAST;
	}
}

int	init_expand_t(t_parse3 *p, t_parse5 *p2, t_token *token)
{
	if (*(p2->var) == 0)
		p->len_var = 0;
	else
		p->len_var = ft_strlen(p2->var);
	p->len_old_var = p2->end - p2->start + 1;
	p->len_s = ft_strlen(token->s) - p->len_old_var + p->len_var;
	p->s = ft_calloc(p->len_s + 1, 1);
	if (!p->s)
		return (1);
	p->i = 0;
	p->k = 0;
	p->j = 0;
	return (0);
}

void	init_parse4(t_parse4 *p)
{
	p->i = 0;
	p->j = 0;
	p->d_quote = 0;
	p->s_quote = 0;
	p->len = 0;
}

int	find_token_length(t_token *token, t_parse4 *p)
{
	int	len;

	len = 0;
	while (token->s[p->i])
	{
		if (token->s[p->i] == 39 && p->d_quote == 0)
		{
			len++;
			p->s_quote++;
			if (p->s_quote == 2)
				p->s_quote = 0;
		}
		else if (token->s[p->i] == 34 && p->s_quote == 0)
		{
			len++;
			p->d_quote++;
			if (p->d_quote == 2)
				p->d_quote = 0;
		}
		p->i++;
	}
	return (len);
}
