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
					fd = hd_c(begin->next->s, line);
			}
		}
		begin = begin->next;
	}
	return (0);
}

int	err_mini_parse(t_line *line)
{
	t_token	*begin;
	t_token	*end;
	int		fd;

	(void)fd;
	// print_token(line);
	begin = line->tokens;
	end = last_elem(line);
	// printf("err : %d\n", line->lexer_err);
	if (!line->tokens)
		return (1);
	if (!end || !begin)
		return (1);
	while (begin)
	{
		if (begin->in_subshell % 2 != 0)
		{
			if (begin->in_subshell == -3)
				ft_putstr_fd("mini: syntax error near unexpected token `('\n", STDERR_FILENO);
			else if (begin->in_subshell == -1)
				ft_putstr_fd("mini: syntax error near unexpected token `)'\n", STDERR_FILENO);
			else
				ft_putstr_fd("mini: syntax error near unexpected token `)'\n", STDERR_FILENO);
			return (line->prev_exit = 2, 1);
		}
		else if (begin->type == REDIR_APPEND || begin->type == REDIR_IN || begin->type == REDIR_OUT || begin->type == HEREDOC)
		{
			if (!begin->next)
			{
				ft_putstr_fd("mini: syntax error near unexpected token `newline'\n", STDERR_FILENO);
				return (line->prev_exit = 2, 1);
			}
			else
			{
				if (begin->next->type != WORD)
				{
					ft_putstr_fd("mini: syntax error near unexpected token `", STDERR_FILENO);
					ft_putstr_fd(begin->next->s, STDERR_FILENO);
					ft_putstr_fd("\'\n", STDERR_FILENO);
					return (line->prev_exit = 2, 1);
				}
				else
				{
					if (begin->type == HEREDOC)
					{
						(void)fd;
					}
				}
			}
		}
		else if (begin->type == PIPE)
		{
			if (!begin->next)
			{
				ft_putstr_fd("mini: syntax error near unexpected token `|'\n", STDERR_FILENO);
				return (line->prev_exit = 2, 1);
			}
			else
			{
				if (begin->next->type == AND || begin->next->type == PIPE || begin->next->type == OR)
				{
					ft_putstr_fd("mini: syntax error near unexpected token `", STDERR_FILENO);
					ft_putstr_fd(begin->next->s, STDERR_FILENO);
					ft_putstr_fd("\'\n", STDERR_FILENO);
					return (line->prev_exit = 2, 1);
				}
			}
		}
		else if (begin->type == OR)
		{
			if (!begin->next)
			{
				ft_putstr_fd("mini: syntax error near unexpected token `||'\n", STDERR_FILENO);
				return (line->prev_exit = 2, 1);
			}
			else
			{
				if (begin->next->type == AND || begin->next->type == PIPE || begin->next->type == OR)
				{
					ft_putstr_fd("mini: syntax error near unexpected token `", STDERR_FILENO);
					ft_putstr_fd(begin->next->s, STDERR_FILENO);
					ft_putstr_fd("\'\n", STDERR_FILENO);
					return (line->prev_exit = 2, 1);
				}
			}
		}
		else if (begin->type == AND)
		{
			if (!begin->next)
			{
				ft_putstr_fd("mini: syntax error near unexpected token `||'\n", STDERR_FILENO);
				return (line->prev_exit = 2, 1);
			}
			else
			{
				if (begin->next->type == AND || begin->next->type == PIPE || begin->next->type == OR)
				{
					ft_putstr_fd("mini: syntax error near unexpected token `", STDERR_FILENO);
					ft_putstr_fd(begin->next->s, STDERR_FILENO);
					ft_putstr_fd("\'\n", STDERR_FILENO);
					return (line->prev_exit = 2, 1);
				}
			}
		}
		begin = begin->next;
	}
	if (line->lexer_err == -8)
	{
		ft_putstr_fd("mini: syntax error near unexpected token `('\n", STDERR_FILENO);
		return (line->prev_exit = 2, 1);
	}
	if (line->lexer_err == -9)
	{
		ft_putstr_fd("mini: syntax error near unexpected token `)'\n", STDERR_FILENO);
		return (line->prev_exit = 2, 1);
	}
	return (0);
}

int	lexer_input(t_line *line)
{
	char	*s;
	char	*start;
	char	*end;

	s = line->input;
	start = s;
	end = NULL;
	if (!s)
		return (line->last_exit);
	while (*s)
	{
		if (*s && is_something(*s))
		{
			s = lexer_input_something(line, s, start, end);
			if (line->last_exit != 0)
				return (line->last_exit);
			if (*s && *(s + 1))
				start = s + 1;
			else
				start = s;
		}
		if (*s)
			s++;
		end = s;
	}
	if (*s == 0 && *start != 0)
	{
		s = lexer_last_char(line, s, start, end);
		if (line->last_exit != 0)
			return (line->last_exit);
	}
	if (err_mini_parse(line) == 1)
	{
		err_open_heredoc(line);
		return (1);
	}
	return (lexer_token(line));
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
