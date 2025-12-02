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

int	handle_expand_and_quote(t_line *line, t_token *temp)
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

int	add_word_to_args(t_expr1 *exp, int i, int *j, t_token *temp)
{
	if (temp->s[0] == '\0')
		return (0);
	exp->new->pipeline[i].args[*j] = ft_strdup(temp->s);
	if (!exp->new->pipeline[i].args[*j])
		return (EX_GEN);
	(*j)++;
	return (0);
}

void	set_from_fd(t_expr1 *exp, int i, int j, t_token *temp)
{
	if (temp->type == REDIR_IN || temp->type == HEREDOC)
		exp->new->pipeline[i].redirect[j].from_fd = 0;
	else
		exp->new->pipeline[i].redirect[j].from_fd = 1;
}

int	handle_heredoc(t_line *line, t_expr1 *exp,
						t_token *temp, t_parse2 *p)
{
	exp->new->pipeline[p->i].redirect[*p->j].redir = ft_strdup("<<");
	if (!exp->new->pipeline[p->i].redirect[*p->j].redir)
		return (EX_GEN);
	if (temp->next && temp->next->type == WORD
		&& temp->next->in_subshell == 0)
		exp->new->pipeline[p->i].redirect[*p->j].hd_fd
			= hd_c(temp->next->s, line);
	else
		exp->new->pipeline[p->i].redirect[*p->j].hd_fd = -1;
	return (0);
}

int	set_redir_and_hd(t_line *line, t_expr1 *exp,
							t_token *temp, t_parse2 *p)
{
	if (temp->type == REDIR_IN)
		exp->new->pipeline[p->i].redirect[*p->j].redir = ft_strdup("<");
	else if (temp->type == HEREDOC)
		return (handle_heredoc(line, exp, temp, p));
	else if (temp->type == REDIR_APPEND)
		exp->new->pipeline[p->i].redirect[*p->j].redir = ft_strdup(">>");
	else
		exp->new->pipeline[p->i].redirect[*p->j].redir = ft_strdup(">");
	if (!exp->new->pipeline[p->i].redirect[*p->j].redir)
		return (EX_GEN);
	return (0);
}
