/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:24 by nburgevi          #+#    #+#             */
/*   Updated: 2025/08/26 04:38:28 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    init_minishell(t_line *line, char **envp)
{

    init_line(line, envp);
    line->last_exit = lexer_input(line);
}

void    init_line(t_line *line, char **envp)
{
    line->tokens = NULL;
    line->exprs = NULL;
    line->last_exit = 0;
    line->num_expr = 0;
    if (envp)
        line->envp = envp;
}

t_expr  *init_new_expr(t_line *line, t_token_type op_ctrl)
{
    t_expr      *new;
    t_token     *temp;
    int         pipe;
    int         i;

    new = ft_calloc(1, sizeof(t_expr));
    if (!new)
        return (NULL);
    new->op_after = op_ctrl;
    temp = line->tokens;
    pipe = 0;
    i = 0;
    while(temp && temp->type != op_ctrl)
    {
        if (temp->type == PIPE)
            pipe++;
        i++;
        temp = temp->next;
    }
    new->pipe_count = pipe;
    new->pipeline = ft_calloc(new->pipe_count + 1, sizeof(t_pipeline));
    if (!new->pipeline)
        return (free(new), NULL);
    return (new);
}

char    **init_pipeline_args(t_line *line, int i)
{
    char    **args;

    args = NULL;
    if (i > 0)
    {
        args = ft_calloc(i + 1, sizeof(char *));
        if (!args)
            return (line->last_exit = 1, NULL);
        args[i] = NULL;
    }
    return (args);
}

t_redir *init_pipeline_redir(t_line *line, int i)
{
    t_redir *redirect;

    redirect = NULL;
    if (i > 0)
    {
        redirect = ft_calloc(i + 1, sizeof(t_redir));
        if (!redirect)
            return (line->last_exit = 1, NULL);
    }
    return (redirect);
}

t_assign    *init_pipeline_assign(t_line *line, int i)
{
    t_assign    *assign;

    assign = NULL;
    if (i > 0)
    {
        assign = ft_calloc(i + 1, sizeof(t_assign));
        if (!assign)
            return (line->last_exit = 1, NULL);
    }
    return (assign);
}

t_pipeline  init_pipeline(t_line *line, int (*len)[3])
{
    t_pipeline  pipeline = {0};
    
    pipeline.args = init_pipeline_args(line, (*len)[0]);
    if (line->last_exit != 0)
        return (pipeline);
    pipeline.redirect = init_pipeline_redir(line, (*len)[1]);
    if (line->last_exit != 0)
    {
            if (pipeline.args)
                free(pipeline.args);
            return (pipeline);
    }
    pipeline.assign = init_pipeline_assign(line, (*len)[2]);
    if (line->last_exit != 0)
    {
        if(pipeline.args)
            free(pipeline.args);
        if(pipeline.redirect)
            free(pipeline.redirect);
        return (pipeline);
    }
    pipeline.word_count = (*len)[0];
    pipeline.redir_count = (*len)[1];
    pipeline.assign_count = (*len)[2];
    ft_bzero(*len, sizeof(*len));
    return (pipeline);
}