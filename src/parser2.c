/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:31 by nburgevi          #+#    #+#             */
/*   Updated: 2025/08/26 04:38:34 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    count_token(t_token *temp, int (*len)[3], t_token_type op_ctrl)
{
    t_token    *temp2;

    temp2 = temp;
    while(temp2 && temp2->type != PIPE && temp2->type != op_ctrl)
    {
        if (temp2->type == WORD && temp2->in_subshell == 0)
        {
            if (!temp2->next)
                (*len)[0]++;
            else if (temp2->next && temp2->next->type != REDIR_IN && temp2->next->type != REDIR_APPEND && temp2->next->type != REDIR_OUT && temp2->next->type != HEREDOC)
                (*len)[0]++;
        }
        if (temp2->type == REDIR_IN || temp2->type == REDIR_APPEND || temp2->type == REDIR_OUT || temp2->type == HEREDOC)
            (*len)[1]++;
        if (temp2->type == ASSIGNMENT)
            (*len)[2]++;
        temp2 = temp2->next;
    }
}

void  find_pipe_position(t_expr *new, t_token *temp, int i)
{
    if (temp)
    {
        if (new->pipe_count == 0)
            new->pipeline[i].position = NO_PIPE;
        if (new->pipe_count != 0)
        {
            if (i == 0)
                new->pipeline[i].position = FIRST;
            else if (i != 0 && i < new->pipe_count)
                new->pipeline[i].position = MIDDLE;
            else
                new->pipeline[i].position = LAST;
        }
    }
    else if (!temp)
    {
        if (new->pipe_count == 0)
            new->pipeline[i].position = NO_PIPE;
        else
            new->pipeline[i].position = LAST;
    }
}