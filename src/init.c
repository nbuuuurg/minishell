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
    lexer_input(line);
}



void    init_line(t_line *line, char **envp)
{
    line->clean = line->input;
    line->tokens = NULL;
    line->exprs = NULL;
    line->path = get_path(envp);
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
        exit(1); // clean exit - malloc fail
    new->op_after = op_ctrl;
    temp = line->tokens;
    pipe = 0;
    i = 0;
    while(temp && temp->type != op_ctrl)
    {
        if (i == 0 && (!temp->next && (temp->type != ASSIGNMENT && temp->type != WORD)))
            exit(1); // Ne peut pas finir par un operateur, clean exit
        if (temp->type == PIPE)
            pipe++;
        i++;
        temp = temp->next;
    }
    new->pipe_count = pipe;
    new->pipeline = ft_calloc(new->pipe_count + 1, sizeof(t_pipeline));
    if (!new->pipeline)
        exit(1); //clean exit - malloc fail
    return (new);
}

t_pipeline  init_pipeline(t_line *line, int (*len)[3])
{
    t_pipeline  pipeline;
    
    (void)line;
	ft_bzero(&pipeline, sizeof(t_pipeline));
    if ((*len)[0] > 0)
    {
        pipeline.args = ft_calloc((*len)[0] + 1, sizeof(char *));
        if (!pipeline.args)
            exit(1); //clean exit - malloc fail
    }
    if ((*len)[1] > 0)
    {
        pipeline.redirect = ft_calloc((*len)[1] + 1, sizeof(t_redir));
        if (!pipeline.redirect)
            exit(1); //clean exit - malloc fail
    }
    if ((*len)[2] > 0)
    {
        pipeline.assign = ft_calloc((*len)[2] + 1, sizeof(t_assign));
        if (!pipeline.assign)
            exit(1); //clean exit - malloc fail
    }
    if ((*len)[0] > 0)
        pipeline.args[(*len)[0]] = NULL;
    pipeline.word_count = (*len)[0];
    pipeline.redir_count = (*len)[1];
    pipeline.assign_count = (*len)[2];
    ft_bzero(*len, sizeof(*len));
    // printf("len0 : %d | len1 : %d | len2 : %d\n", (*len)[0], (*len)[1], (*len)[2]);
    return (pipeline);
}
