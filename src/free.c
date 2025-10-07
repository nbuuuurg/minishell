/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:17 by nburgevi          #+#    #+#             */
/*   Updated: 2025/08/26 04:38:21 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_line(t_line *line)
{
	if (line->tokens)
		free_tokens(line->tokens);
	if (line->exprs)
		free_exprs(line->exprs);
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

void    free_tokens(t_token *tokens)
{
    t_token *temp;

    temp = tokens;
    while(tokens)
    {
        if (tokens->s)
            free(tokens->s);
        tokens = tokens->next;
        free(temp);
        temp = tokens;
    }
	free(tokens);
}

void    free_exprs(t_expr *exprs)
{
    t_expr  *temp;

    temp = exprs;
    while (exprs)
    {
		if (exprs->pipeline)
            	free_pipeline(exprs->pipeline);
        exprs = exprs->next;
        free(temp);
        temp = exprs;
    }
	free(exprs);
}

void    free_pipeline(t_pipeline *pipe)
{
    int     i;

    if (pipe->args)
		free_split(pipe->args);
    if (pipe->redirect && pipe->redir_count > 0)
    {
		i = 0;
		while (i < pipe->redir_count)
		{
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
		printf("%d\n", pipe->assign_count);
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
	free(pipe);
}