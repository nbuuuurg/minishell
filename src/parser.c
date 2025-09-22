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

t_expr  *parse_new_expr(t_line *line, t_token_type op_ctrl)
{
    int     i;
    int     len[3];
    t_expr  *new;
    t_token *temp;

    new = init_new_expr(line, op_ctrl);
    temp = line->tokens;
    i = 0;
    while (temp && temp->type != op_ctrl)
    {
        ft_bzero(len, sizeof(len));
        count_token(temp, &len, op_ctrl);
        new->pipeline[i] = init_pipeline(line, &len);
        temp = parse_pipeline(line, temp, new, &len, &i);
        if (!temp)
            break ;
        if (temp->type == PIPE)
            temp = temp->next;
        else
            break ;
    }
    new->next = NULL;
    return (new);
}

t_token *parse_pipeline(t_line *line, t_token *temp, t_expr *new, int (*len)[3], int *i)
{
    while (temp && temp->type != PIPE && temp->type != new->op_after)
    {
        if (temp->type == WORD && temp->in_subshell == 0)
            parse_word(line, new, temp, *i, &(*len)[0]);
        else if (temp->type == REDIR_IN || temp->type == REDIR_APPEND || temp->type == REDIR_OUT || temp->type == HEREDOC)
            parse_redir(line, new, temp, *i, &(*len)[1]);
        else if (temp->type == ASSIGNMENT)
            parse_assignment(line, new, temp, *i, &(*len)[2]);
        temp = temp->next;
        find_pipe_position(new, temp, *i);
    }
    (*i)++;
    return (temp);
}

void    parse_word(t_line *line, t_expr *new, t_token *temp, int i, int *j)
{
    (void)line;
    // if (temp->in_subshell == 1)
        //lex subshell
    if (temp->previous && (temp->previous->type == REDIR_IN || temp->previous->type == REDIR_APPEND || temp->previous->type == REDIR_OUT || temp->previous->type == HEREDOC))
        return ;
    new->pipeline[i].args[*j] = ft_strdup(temp->s);
    // printf("temp->s : %s | j = %d\n", temp->s, *j);
    if (!new->pipeline[i].args[*j])
        exit(1); // clean exit - malloc fail
    // printf("new->expr->pipeline[%d].args[%d] = %s\n",i, *j, new->pipeline[i].args[*j]);
    (*j)++;
}

void    parse_redir(t_line *line, t_expr *new, t_token *temp, int i, int *j)
{
    (void)line;
    if (ft_isdigit(temp->s[0]))
        new->pipeline[i].redirect[*j].from_fd = ft_atoi(temp->s);
    else if (temp->type == REDIR_IN || temp->type == HEREDOC)
        new->pipeline[i].redirect[*j].from_fd = 0;
    else
        new->pipeline[i].redirect[*j].from_fd = 1;
    if (temp->type == REDIR_IN)
        new->pipeline[i].redirect[*j].redir = ft_strdup("<");
    else if (temp->type == HEREDOC)
        new->pipeline[i].redirect[*j].redir = ft_strdup("<<");
    else if (temp->type == REDIR_APPEND)
        new->pipeline[i].redirect[*j].redir = ft_strdup(">>");
    else
        new->pipeline[i].redirect[*j].redir = ft_strdup(">");
    if (!new->pipeline[i].redirect[*j].redir)
        exit(1); // clean exit - malloc fail
    if (temp->next && temp->next->type == WORD)
        new->pipeline[i].redirect[*j].file = ft_strdup(temp->next->s);
    else
        exit(1); // clean exit - No word after redirection
    new->pipeline[i].redirect[*j].order = *j;
    // printf("new->pipeline[%d].redirect[%d].from_fd = %d\n", i, *j, new->pipeline[i].redirect[*j].from_fd);
    // printf("new->pipeline[%d].redirect[%d].redir = %s\n", i, *j, new->pipeline[i].redirect[*j].redir);
    // printf("new->pipeline[%d].redirect[%d].file = %s\n", i, *j, new->pipeline[i].redirect[*j].file);
    (*j)++;
    temp = temp->next;
}

void    parse_assignment(t_line *line, t_expr *new, t_token *temp, int i, int *j)
{
    (void)line;
    new->pipeline[i].assign[*j].name = dup_assign_name(temp->s);
    if (!new->pipeline[i].assign[*j].name)
        exit(1); //clean exit - malloc fail
    new->pipeline[i].assign[*j].value = dup_assign_value(temp->s);
    if (!new->pipeline[i].assign[*j].value)
        exit(1); //clean exit - malloc fail
    // printf("new->pipeline[%d].assign[%d].name = %s\n", i, *j, new->pipeline[i].assign[*j].name);
    // printf("new->pipeline[%d].assign[%d].value = %s\n", i, *j, new->pipeline[i].assign[*j].value);
    (*j)++; 
}