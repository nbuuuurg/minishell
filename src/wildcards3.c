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

t_token	*lst_join_single(t_token *at, t_token *nw, t_line *line)
{
	free(at->s);
	free(at);
	line->tokens = nw;
	return (nw);
}

t_token	*lst_join_between(t_token *left, t_token *nw, t_token *right)
{
	t_token	*tail;

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

t_token	*lst_join(t_token *at, t_token *nw, t_line *line)
{
	t_token	*left;
	t_token	*right;

	if (!at || !nw)
		return (at);
	left = at->previous;
	right = at->next;
	if (!left && !right)
		return (lst_join_single(at, nw, line));
	free(at->s);
	free(at);
	return (lst_join_between(left, nw, right));
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
