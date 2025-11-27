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

t_token	*p_pipe(t_line *line, t_expr1 *exp)
{
	t_token	*temp;

	temp = line->tokens;
	while (temp && temp->type != PIPE && temp->type != exp->new->op_after)
	{
		if (!process_token(line, exp, temp))
			return (NULL);
		if (line->last_exit != 0)
			return (NULL);
		if (line->heredoc_flag == 1)
			break ;
		temp = temp->next;
		find_pipe_position(exp->new, temp, exp->i);
	}
	exp->i++;
	return (temp);
}

int	parse_word(t_line *line, t_expr1 *exp, t_token *temp)
{
	int	i;
	int	*j;
	int	ret;

	i = exp->i;
	j = &exp->len[0];
	if (prev_is_redir(temp))
		return (0);
	ret = handle_expand_and_quote(line, temp);
	if (ret == EX_GEN)
		return (EX_GEN);
	ret = add_word_to_args(exp, i, j, temp);
	if (ret == EX_GEN)
		return (EX_GEN);
	return (0);
}

int	parse_redir(t_line *line, t_expr1 *exp, t_token *temp)
{
	t_parse2	p;

	p.i = exp->i;
	p.j = &exp->len[1];
	set_from_fd(exp, p.i, *p.j, temp);
	if (set_redir_and_hd(line, exp, temp, &p) == EX_GEN)
		return (EX_GEN);
	if (set_redirect_file(line, exp, temp, &p) == EX_GEN)
		return (EX_GEN);
	exp->new->pipeline[p.i].redirect[*p.j].order = *p.j;
	(*p.j)++;
	return (0);
}

int	parse_assignment(t_line *line, t_expr1 *exp, t_token *temp)
{
	int	i;
	int	*j;

	(void)line;
	i = exp->i;
	j = &exp->len[2];
	exp->new->pipeline[i].assign[*j].name = dup_assign_name(temp->s);
	if (!exp->new->pipeline[i].assign[*j].name)
		return (EX_GEN);
	exp->new->pipeline[i].assign[*j].value = dup_assign_value(temp->s);
	if (!exp->new->pipeline[i].assign[*j].value)
		return (free(exp->new->pipeline[i].assign[*j].name), 1);
	(*j)++;
	return (0);
}
