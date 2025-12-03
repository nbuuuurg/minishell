/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:44:29 by nburgevi          #+#    #+#             */
/*   Updated: 2025/12/03 11:44:30 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_err_1(t_token *begin)
{
	if (begin->in_subshell == -3)
	{
		ft_putstr_fd("mini: syntax error near unexpected token `('\n",
			STDERR_FILENO);
		return (1);
	}
	else if (begin->in_subshell == -1)
	{
		ft_putstr_fd("mini: syntax error near unexpected token `)'\n",
			STDERR_FILENO);
		return (1);
	}
	else
	{
		ft_putstr_fd("mini: syntax error near unexpected token `)'\n",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	print_err_2(t_token *begin)
{
	int	fd;

	fd = 0;
	if (!begin->next)
	{
		ft_putstr_fd("mini: syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
		return (1);
	}
	else
	{
		if (begin->next->type != WORD)
		{
			ft_putstr_fd("mini: syntax error near unexpected token `",
				STDERR_FILENO);
			ft_putstr_fd(begin->next->s, STDERR_FILENO);
			ft_putstr_fd("\'\n", STDERR_FILENO);
			return (1);
		}
		else
			return ((void)fd, 0);
	}
	return (0);
}

int	print_err_3(t_token *begin)
{
	if (!begin->next)
	{
		ft_putstr_fd("mini: syntax error near unexpected token `|'\n",
			STDERR_FILENO);
		return (1);
	}
	else
	{
		if (begin->next->type == AND || begin->next->type == PIPE
			|| begin->next->type == OR)
		{
			ft_putstr_fd("mini: syntax error near unexpected token `",
				STDERR_FILENO);
			ft_putstr_fd(begin->next->s, STDERR_FILENO);
			ft_putstr_fd("\'\n", STDERR_FILENO);
			return (1);
		}
		else
			return (0);
	}
	return (0);
}

int	print_err_4(t_token *begin)
{
	if (!begin->next)
	{
		ft_putstr_fd("mini: syntax error near unexpected token `||'\n",
			STDERR_FILENO);
		return (1);
	}
	else
	{
		if (begin->next->type == AND || begin->next->type == PIPE
			|| begin->next->type == OR)
		{
			ft_putstr_fd("mini: syntax error near unexpected token `",
				STDERR_FILENO);
			ft_putstr_fd(begin->next->s, STDERR_FILENO);
			ft_putstr_fd("\'\n", STDERR_FILENO);
			return (1);
		}
		else
			return (0);
	}
	return (0);
}

int	print_err_5(t_token *begin, t_line *line)
{
	if (!begin->next)
	{
		ft_putstr_fd("mini: syntax error near unexpected token `||'\n",
			STDERR_FILENO);
		return (1);
	}
	else
	{
		if (begin->next->type == AND || begin->next->type == PIPE
			|| begin->next->type == OR)
		{
			ft_putstr_fd("mini: syntax error near unexpected token `",
				STDERR_FILENO);
			ft_putstr_fd(begin->next->s, STDERR_FILENO);
			ft_putstr_fd("\'\n", STDERR_FILENO);
			return (1);
		}
		else
			return (0);
	}
	if (lex_err(line) == 1)
		return (1);
	return (0);
}
