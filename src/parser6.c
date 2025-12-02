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

int	handle_word(t_line *line, t_expr1 *exp, t_token *temp)
{
	char	**args;

	line->last_exit = parse_word(line, exp, temp);
	if (line->last_exit != 0)
		return (0);
	if (!need_split_expand(temp))
		return (1);
	args = d_w_t(line, exp, temp);
	exp->new->pipeline[exp->i].args = args;
	if (!args)
	{
		line->last_exit = 1;
		return (0);
	}
	return (1);
}

void	handle_subshell(t_line *line, t_expr1 *exp, t_token *temp)
{
	exp->new->has_subshell = temp->in_subshell;
	line->last_exit = 0;
}

void	handle_redir_or_assign(t_line *line, t_expr1 *exp, t_token *temp)
{
	if (temp->type == REDIR_IN || temp->type == REDIR_APPEND
		|| temp->type == REDIR_OUT || temp->type == HEREDOC)
		line->last_exit = parse_redir(line, exp, temp);
	else if (temp->type == ASSIGNMENT)
		line->last_exit = parse_assignment(line, exp, temp);
}

int	process_token(t_line *line, t_expr1 *exp, t_token *temp)
{
	if (temp->in_subshell != 0)
		handle_subshell(line, exp, temp);
	else if (temp->type == WORD)
	{
		if (!handle_word(line, exp, temp))
			return (0);
	}
	else if (temp->type == REDIR_IN || temp->type == REDIR_APPEND
		|| temp->type == REDIR_OUT || temp->type == HEREDOC
		|| temp->type == ASSIGNMENT)
		handle_redir_or_assign(line, exp, temp);
	return (1);
}

int	prev_is_redir(t_token *temp)
{
	if (temp->previous
		&& (temp->previous->type == REDIR_IN
			|| temp->previous->type == REDIR_APPEND
			|| temp->previous->type == REDIR_OUT
			|| temp->previous->type == HEREDOC))
		return (1);
	return (0);
}
