/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 01:34:08 by adeflers          #+#    #+#             */
/*   Updated: 2025/11/05 08:02:32 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	init_subshell(t_line *line, t_token *subinput)
{
	t_line	*subline;

	subline = dup_line(line, subinput);
	if (!subline)
		return (EX_GEN);
	subline->last_exit = lexer_input(subline);
	if (subline->last_exit != 0)
		return (free_line(subline), EX_GEN);
	if (subline->heredoc_flag == 1)
	{
		free_line(subline);
		free(subline);
		return (-1);
	}
	if (subline->lexer_err != 0)
		line->lexer_err = -3;
	line->prev_exit = subline->prev_exit;
	if (subline->envp)
		free_split(subline->envp);
	free_line(subline);
	free(subline);
	return (0);
}

t_line	*dup_line(t_line *line, t_token *subinput)
{
	t_line	*subline;

	subline = ft_calloc(1, sizeof(t_line));
	if (!subline)
		return (line->last_exit = EX_GEN, NULL);
	subline->input = new_subinput(line, subinput);
	if (!subline->input)
		return (line->last_exit = EX_GEN, NULL);
	subline->clean = subline->input;
	subline->envp = ft_strdup2(line->envp);
	if (!subline->envp)
		return (line->last_exit = EX_GEN, NULL);
	subline->tokens = NULL;
	subline->exprs = NULL;
	subline->cmd = NULL;
	subline->prev_exit = line->prev_exit;
	subline->subline = line;
	subline->last_exit = 0;
	subline->len = line->len - 2;
	subline->lexer_err = 0;
	subline->num_expr = 0;
	subline->heredoc_flag = line->heredoc_flag;
	subline->path = get_path(subline->envp);
	return (subline);
}
