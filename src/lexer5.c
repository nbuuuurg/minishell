/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:24 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:03:12 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	lexer_finish(t_line *line)
{
	if (err_mini_parse(line) == 1)
	{
		err_open_heredoc(line);
		return (1);
	}
	return (lexer_token(line));
}

int	lexer_input(t_line *line)
{
	char	*s;
	char	*start;
	char	*end;
	int		ret;

	s = line->input;
	start = s;
	end = NULL;
	if (!s)
		return (line->last_exit);
	while (*s)
	{
		ret = lexer_process_char(line, &s, &start, &end);
		if (ret != 0)
			return (ret);
	}
	ret = lexer_handle_last_char(line, &s, &start, &end);
	if (ret != 0)
		return (ret);
	return (lexer_finish(line));
}

int	l_split_expr(t_line *line, t_lexer1 *lexer)
{
	if (lexer->temp->type == AND || lexer->temp->type == OR)
	{
		if (lexer->i == 0 || !lexer->temp->next)
			return (line->lexer_err = -4, 0);
		lexer->new = parse_new_expr(line, lexer->temp->type);
		if (!lexer->new)
			return (EX_GEN);
		if (!line->exprs)
			line->exprs = lexer->new;
		else
		{
			while (lexer->expr->next)
				lexer->expr = lexer->expr->next;
			lexer->expr->next = lexer->new;
		}
		line->tokens = lexer->temp->next;
	}
	if (lexer->flag_2 == 0)
		exec_exprs(lexer->new, line->path, line);
	return (0);
}

int	l_single_expr(t_line *line, t_expr *new, t_expr *expr)
{
	new = parse_new_expr(line, 9);
	if (!new)
		return (line->last_exit = EX_GEN);
	if (!line->exprs)
		line->exprs = new;
	else
	{
		while (expr->next)
			expr = expr->next;
		expr->next = new;
		new->next = NULL;
	}
	exec_exprs(new, line->path, line);
	return (0);
}
