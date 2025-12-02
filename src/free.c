/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:17 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 07:55:36 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_line_fork(t_line *line, int i)
{
	if (!line)
		return ;
	if (line->tokens)
		free_tokens(line->tokens);
	if (line->exprs)
		free_e(line->exprs);
	if (line->path)
		free_split(line->path);
	if (line->envp)
		free_split(line->envp);
	free(line->input);
	if (line->subline)
	{
		if (line->subline->subline)
			free_line_fork(line->subline, 0);
		else
			free_line_fork(line->subline, 1);
	}
	if (i == 0)
		free(line);
}

void	free_line(t_line *line)
{
	if (!line)
		return ;
	if (line->tokens)
		free_tokens(line->tokens);
	if (line->exprs)
		free_e(line->exprs);
	if (line->path)
		free_split(line->path);
	free(line->input);
}

void	free_split(char **s)
{
	char	**temp;

	temp = s;
	while (temp && *temp)
	{
		free(*temp);
		temp++;
	}
	free(s);
}

void	free_split2(char **s)
{
	char	**temp;

	temp = s;
	while (temp && *temp)
	{
		free(*temp);
		temp++;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	if (!tokens)
		return ;
	if (tokens->previous)
	{
		while (tokens->previous)
			tokens = tokens->previous;
	}
	temp = tokens;
	while (tokens)
	{
		if (tokens->s)
			free(tokens->s);
		tokens = tokens->next;
		free(temp);
		temp = tokens;
	}
	free(tokens);
}
