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

int	err_open_heredoc(t_line *line)
{
	t_token	*begin;
	int		fd;

	(void)line;
	begin = line->tokens;
	while (begin)
	{
		if (begin->in_subshell != 0)
			(void)fd;
		else if (begin->type == HEREDOC)
		{
			if (!begin->next)
				(void)fd;
			else
			{
				if (begin->next->type == WORD)
				{
					fd = hd_c(begin->next->s, line);
					close(fd);
				}
			}
		}
		begin = begin->next;
	}
	return (0);
}

int	lex_err(t_line *line)
{
	if (line->lexer_err == -8)
	{
		ft_putstr_fd("mini: syntax error near unexpected token `('\n", STDERR_FILENO);
		return (1);
	}
	if (line->lexer_err == -9)
	{
		ft_putstr_fd("mini: syntax error near unexpected token `)'\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	err_mini_parse(t_line *line)
{
	t_token	*begin;

	begin = line->tokens;
	if (!line->tokens || !begin)
		return (1);
	while (begin)
	{
		if (begin->in_subshell % 2 != 0)
			return (print_err_1(begin), line->prev_exit = 2, 1);
		else if (begin->type == REDIR_APPEND || begin->type == REDIR_IN || begin->type == REDIR_OUT || begin->type == HEREDOC)
			return (print_err_2(begin), line->prev_exit = 2, 1);
		else if (begin->type == PIPE)
			return (print_err_3(begin), line->prev_exit = 2, 1);
		else if (begin->type == OR)
			return (print_err_4(begin), line->prev_exit = 2, 1);
		else if (begin->type == AND)
			return (print_err_5(begin), line->prev_exit = 2, 1);
		begin = begin->next;
	}
	if (lex_err(line) == 1)
		return (line->prev_exit = 2, 1);
	return (0);
}

int	lexer_process_char(t_line *line, char **s, char **start, char **end)
{
	if (**s && is_something(**s))
	{
		*s = lexer_input_something(line, *s, *start, *end);
		if (line->last_exit != 0)
			return (line->last_exit);
		if (**s && *(*s + 1))
			*start = *s + 1;
		else
			*start = *s;
	}
	if (**s)
		(*s)++;
	*end = *s;
	return (0);
}

int	lexer_handle_last_char(t_line *line, char **s, char **start, char **end)
{
	if (**s == 0 && **start != 0)
	{
		*s = lexer_last_char(line, *s, *start, *end);
		if (line->last_exit != 0)
			return (line->last_exit);
	}
	return (0);
}

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

int	lexer_token(t_line *line)
{
	int		i;
	t_token	*temp;
	t_expr	*expr;
	t_expr	*new;
	int		flag;
	int		flag_2;

	expr = NULL;
	new = NULL;
	i = 0;
	flag = 0;
	flag_2 = 0;
	temp = line->tokens;
	while (temp)
	{
		expr = line->exprs;
		if (temp->in_subshell > 0 && flag_2 == 0)
		{
			if (line->lexer_err == -2)
				return (line->last_exit);
			line->last_exit = init_subshell(line, temp);
			if (line->last_exit != 0)
				return (line->last_exit);
			if (line->heredoc_flag == 1)
			{
				line->prev_exit = 130;
				return (0);
			}
			flag = 1;
			new = ft_calloc(1, sizeof(t_expr));
			if (!new)
				return (line->last_exit = 1);
			ft_bzero(new, sizeof(t_expr));
			new->has_subshell = 1;
			if (!line->exprs)
				line->exprs = new;
			else
			{
				while (expr->next)
					expr = expr->next;
				expr->next = new;
			}
			while (temp->next && temp->next->type != OR
				&& temp->next->type != AND && temp->next->in_heredoc == 0)
				temp = temp->next;
		}
		if (temp->has_wildcards == 1)
		{
			temp = parse_wildcards(line, temp);
			if (!temp)
				return (line->last_exit);
		}
		if ((temp->type == AND || temp->type == OR) && flag == 0)
		{
			line->last_exit = l_split_expr(line, temp, new, expr, i, flag_2);
			if (temp->type == OR && line->prev_exit == 0)
				flag_2 = 1;
			else
				flag_2 = 0;
		}
		if (line->last_exit != 0)
			return (line->last_exit);
		temp = temp->next;
		if (!temp && flag == 0 && flag_2 == 0)
			line->last_exit = l_single_expr(line, new, expr);
		i++;
		if (line->last_exit != 0)
			return (line->last_exit);
		flag = 0;
	}
	return (0);
}

int	l_split_expr(t_line *line, t_token *temp, t_expr *new, t_expr *expr, int i, int exec)
{
	if (temp->type == AND || temp->type == OR)
	{
		if (i == 0 || !temp->next)
			return (line->lexer_err = -4, 0);
		new = parse_new_expr(line, temp->type);
		if (!new)
			return (EX_GEN);
		if (!line->exprs)
			line->exprs = new;
		else
		{
			while (expr->next)
				expr = expr->next;
			expr->next = new;
		}
		line->tokens = temp->next;
	}
	if (exec == 0)
		exec_exprs(new, line->path, line);
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
