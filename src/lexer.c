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
				if (begin->next->type == WORD && line->heredoc_flag == 0)
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
		ft_putstr_fd("mini: syntax error near unexpected token `('\n",
			STDERR_FILENO);
		return (1);
	}
	if (line->lexer_err == -9)
	{
		ft_putstr_fd("mini: syntax error near unexpected token `)'\n",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	err_mini_parse(t_line *line)
{
	t_token	*begin;
	int		i;

	i = 0;
	begin = line->tokens;
	if (!line->tokens || !begin)
		return (1);
	while (begin)
	{
		if (begin->in_subshell % 2 != 0)
			i = print_err_1(begin);
		else if (begin->type == REDIR_APPEND || begin->type == REDIR_IN
			|| begin->type == REDIR_OUT || begin->type == HEREDOC)
			i = print_err_2(begin);
		else if (begin->type == PIPE)
			i = print_err_3(begin);
		else if (begin->type == OR)
			i = print_err_4(begin);
		else if (begin->type == AND)
			i = print_err_5(begin, line);
		begin = begin->next;
		if (i == 1)
			return (line->prev_exit = 2, 1);
	}
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
