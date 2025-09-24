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

void    free_tokens(t_token *token)
{
    t_token *temp;

    temp = token;
    while(token)
    {
        if (token->s)
            free(token->s);
        token = token->next;
        free(temp);
        temp = token;
    }
}

void    free_exprs(t_expr *expr)
{
    t_expr  *temp;
	int		i;

    temp = expr;
    while (expr)
    {
		i = 0;
		if (expr->pipeline && expr->pipe_count > 0)
		{
			while (i < expr->pipe_count)
            	free_pipeline(&expr->pipeline[i++]);
		}
		if (expr->pipeline)
			free(expr->pipeline);
        expr = expr->next;
        free(temp);
        temp = expr;
    }
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