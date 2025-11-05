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
	{
		while (line->tokens->previous)
			line->tokens = line->tokens->previous;
		free_tokens(line->tokens);
	}
	if (line->exprs)
		free_exprs(line->exprs);
	if (line->path)
		free_split(line->path);
	free(line->input);
	if (line->subline)
	{
		if (line->subline->subline)
			free_line_fork(line->subline, 0);
		else
			free_line_fork(line->subline, 1);
	}
	else
		free_line_fork(line->subline, 1);
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
		free_exprs(line->exprs);
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

	temp = tokens;
	if (!tokens)
		return ;
	while (tokens)
	{
		if (tokens->s)
		{
			free(tokens->s);
		}
		tokens = tokens->next;
		free(temp);
		temp = tokens;
	}
	free(tokens);
}

void	free_exprs(t_expr *exprs)
{
	t_expr	*temp;
	int		i;

	temp = exprs;
	while (exprs)
	{
		i = exprs->pipe_count;
		while (i >= 0)
		{
			free_pipeline(&exprs->pipeline[i]);
			i--;
		}
		if (exprs->pipeline)
			free(exprs->pipeline);
		exprs = exprs->next;
		free(temp);
		temp = exprs;
	}
	free(exprs);
}

void	free_pipeline(t_pipeline *pipe)
{
	int	i;

	if (!pipe)
		return ;
	if (pipe->args)
		free_split(pipe->args);
	if (pipe->redirect && pipe->redir_count > 0)
	{
		i = 0;
		while (i < pipe->redir_count)
		{
			if (pipe->redirect[i].redir
				&& pipe->redirect[i].redir[0] == '<'
				&& pipe->redirect[i].redir[1] == '<'
				&& pipe->redirect[i].hd_fd >= 0)
				close(pipe->redirect[i].hd_fd);
			if (pipe->redirect[i].redir)
				free(pipe->redirect[i].redir);
			if (pipe->redirect[i].file)
				free(pipe->redirect[i].file);
			i++;
		}
		free(pipe->redirect);
	}
	if (pipe->assign && pipe->assign_count > 0)
	{
		i = 0;
		while (i < pipe->assign_count)
		{
			if (pipe->assign[i].name)
				free(pipe->assign[i].name);
			if (pipe->assign[i].value)
				free(pipe->assign[i].value);
			i++;
		}
		free(pipe->assign);
	}
}

void	free_cmd_path(t_line *line)
{
	int	i;

	i = line->cmd->pipe_count;
	while (i >= 0)
	{
		if (line->cmd && line->cmd[i].full_path)
			free(line->cmd[i].full_path);
		i--;
	}
}
