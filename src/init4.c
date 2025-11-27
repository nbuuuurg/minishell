/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:24 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:00:00 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_token(t_token *token, int multiple_quote, int quote, int i)
{
	token->s[i] = 0;
	token->quoted = def_quote(multiple_quote, quote);
	token->previous = NULL;
	token->next = NULL;
	token = token_type(token);
	token = has_expand(token);
}

t_expr	*init_new_expr(t_line *line, t_token_type op_ctrl)
{
	t_expr	*new;
	t_token	*temp;
	int		pipe;
	int		i;

	new = ft_calloc(1, sizeof(t_expr));
	if (!new)
		return (NULL);
	new->op_after = op_ctrl;
	temp = line->tokens;
	pipe = 0;
	i = 0;
	while (temp && temp->type != op_ctrl)
	{
		if (temp->type == PIPE)
			pipe++;
		i++;
		temp = temp->next;
	}
	new->pipe_count = pipe;
	new->has_subshell = 0;
	new->pipeline = ft_calloc(new->pipe_count + 1, sizeof(t_pipeline));
	if (!new->pipeline)
		return (free(new), NULL);
	return (new);
}
