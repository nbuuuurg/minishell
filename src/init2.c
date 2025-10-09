/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 15:15:10 by nburgevi          #+#    #+#             */
/*   Updated: 2025/10/09 15:15:21 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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