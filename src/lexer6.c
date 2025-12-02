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

void	l_init_lexer1(t_lexer1 *lexer, t_line *line)
{
	lexer->i = 0;
	lexer->temp = line->tokens;
	lexer->expr = NULL;
	lexer->new = NULL;
	lexer->flag = 0;
	lexer->flag_2 = 0;
}

void	handle_new_subshell_expr(t_lexer1 *lexer, t_line *line)
{
	if (!line->exprs)
		line->exprs = lexer->new;
	else
	{
		while (lexer->expr->next)
			lexer->expr = lexer->expr->next;
		lexer->expr->next = lexer->new;
	}
	while (lexer->temp->next && lexer->temp->next->type != OR
		&& lexer->temp->next->type != AND
		&& lexer->temp->next->in_heredoc == 0)
		lexer->temp = lexer->temp->next;
}

int	l_handle_subshell(t_line *line, t_lexer1 *lexer)
{
	if (lexer->temp->in_subshell <= 0 || lexer->flag_2 != 0)
		return (0);
	if (line->lexer_err == -2)
		return (line->last_exit);
	line->last_exit = init_subshell(line, lexer->temp);
	if (line->last_exit != 0)
		return (line->last_exit);
	if (line->heredoc_flag == 1)
	{
		line->prev_exit = 130;
		return (-1);
	}
	lexer->flag = 1;
	lexer->new = ft_calloc(1, sizeof(t_expr));
	if (!lexer->new)
		return (line->last_exit = 1);
	ft_bzero(lexer->new, sizeof(t_expr));
	lexer->new->has_subshell = 1;
	handle_new_subshell_expr(lexer, line);
	return (0);
}

int	l_handle_token(t_line *line, t_lexer1 *lexer)
{
	int	ret;

	lexer->expr = line->exprs;
	ret = l_handle_subshell(line, lexer);
	if (ret != 0)
		return (ret);
	if (lexer->temp->has_wildcards == 1)
		lexer->temp = parse_wildcards(line, lexer->temp);
	if ((lexer->temp->type == AND || lexer->temp->type == OR)
		&& lexer->flag == 0)
	{
		line->last_exit = l_split_expr(line, lexer);
		if (lexer->temp->type == OR && line->prev_exit == 0)
			lexer->flag_2 = 1;
		else
			lexer->flag_2 = 0;
	}
	lexer->temp = lexer->temp->next;
	if (!lexer->temp && lexer->flag == 0 && lexer->flag_2 == 0)
		line->last_exit = l_single_expr(line, lexer->new, lexer->expr);
	lexer->i++;
	if (line->last_exit != 0)
		return (line->last_exit);
	lexer->flag = 0;
	return (0);
}

int	lexer_token(t_line *line)
{
	t_lexer1	lexer;
	int			ret;

	l_init_lexer1(&lexer, line);
	while (lexer.temp)
	{
		ret = l_handle_token(line, &lexer);
		if (ret == -1)
			return (0);
		if (ret != 0)
			return (ret);
	}
	return (0);
}
