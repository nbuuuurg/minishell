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
    if (!new)
        return (NULL);
    temp = line->tokens;
    i = 0;
    while (temp && temp->type != op_ctrl)
    {
        ft_bzero(len, sizeof(len));
        count_token(temp, &len, op_ctrl);
        new->pipeline[i] = init_pipeline(line,  &len);
        if (line->last_exit != 0)
            return (free_pipeline(&new->pipeline[i]), free_exprs(new), NULL);
        temp = parse_pipeline(line, temp, new, &len, &i);
        if (line->last_exit != 0)
            return (free_pipeline(&new->pipeline[i]), free_exprs(new), NULL);
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
        if (temp->in_subshell != 0)
        {
            new->has_subshell = temp->in_subshell;
            line->last_exit = 0;
        }
        else if (temp->type == WORD)
            line->last_exit = parse_word(line, new, temp, *i, &(*len)[0]);
        else if (temp->type == REDIR_IN || temp->type == REDIR_APPEND || temp->type == REDIR_OUT || temp->type == HEREDOC)
            line->last_exit = parse_redir(line, new, temp, *i, &(*len)[1]);
        else if (temp->type == ASSIGNMENT)
            line->last_exit = parse_assignment(line, new, temp, *i, &(*len)[2]);
        if (line->last_exit != 0)
            return (NULL);
        temp = temp->next;
        find_pipe_position(new, temp, *i);
    }
    (*i)++;
    return (temp);
}

// t_token     *parse_wildcards(t_line *line, t_token *token)
// {
//     t_token **temp;
//     DIR *dir;
//     size_t     len_entry;
//     struct dirent   *entry;

//     dir = opendir(".");
//     if (!dir)
//         return (line->last_exit = 1, NULL);
//     len_entry = 0;
//     while((entry = readdir(dir)) != NULL)
//         len_entry++;
//     if (closedir(dir) == -1)
//         return (line->last_exit = 1, NULL);
    
//     temp = ft_calloc(len_entry, sizeof(t_token *));
//     if (!temp)
//         return (line->last_exit = 1, NULL);
//     dir = opendir(".");
//     if (!dir)
//         return (line->last_exit = 1, NULL);
//     while((entry = readdir(dir)) != NULL)
//     {
//         if (is_a_match(entry->d_name, token->s))
//             // Si il y a match, il faut remplacer le token par une liste de token et bien tout chainer
//     }
//     return (token);
// }

int    parse_word(t_line *line, t_expr *new, t_token *temp, int i, int *j)
{
    (void)line;
    if (temp->previous && (temp->previous->type == REDIR_IN || temp->previous->type == REDIR_APPEND || temp->previous->type == REDIR_OUT || temp->previous->type == HEREDOC))
        return (0);
    // if (temp->has_wildcards)
    // {
    //     temp = parse_wildcards(line, temp);
    //     if (line->last_exit == EX_GEN)
    //         return (EX_GEN);
    // }
    if (temp->has_expand != 0)
    {
        temp->s = parse_expand(line, temp);
        if (line->last_exit == EX_GEN)
            return (EX_GEN);
    }
    if (temp->quoted != NO_QUOTE)
        temp->s = parse_quoted_token(line, temp);
    if (line->last_exit == EX_GEN)
        return (EX_GEN);
    new->pipeline[i].args[*j] = ft_strdup(temp->s);
    if (!new->pipeline[i].args[*j])
        return (EX_GEN);
    (*j)++;
    return (0);
}

int    parse_redir(t_line *line, t_expr *new, t_token *temp, int i, int *j)
{
    (void)line;
    if (temp->type == REDIR_IN || temp->type == HEREDOC)
        new->pipeline[i].redirect[*j].from_fd = 0;
    else
        new->pipeline[i].redirect[*j].from_fd = 1;
    if (temp->type == REDIR_IN)
        new->pipeline[i].redirect[*j].redir = ft_strdup("<");
    else if (temp->type == HEREDOC)
    {
        new->pipeline[i].redirect[*j].redir = ft_strdup("<<");
        if (!new->pipeline[i].redirect[*j].redir)
            return (EX_GEN);
        if (temp->next && temp->next->type == WORD && temp->next->in_subshell == 0)
            new->pipeline[i].redirect[*j].heredoc_fd = here_doc_content(temp->next->s, line);
        else
            new->pipeline[i].redirect[*j].heredoc_fd = -1;
    }
    else if (temp->type == REDIR_APPEND)
        new->pipeline[i].redirect[*j].redir = ft_strdup(">>");
    else
        new->pipeline[i].redirect[*j].redir = ft_strdup(">");

    if (!new->pipeline[i].redirect[*j].redir)
        return (EX_GEN);
    if (temp->next && temp->next->type == WORD && temp->next->in_subshell == 0)
    {
        new->pipeline[i].redirect[*j].file = ft_strdup(temp->next->s);
        if (temp->next->type != WORD || temp->next->in_subshell != 0)
            line->lexer_err = -8; // delimiteur non valide
    }
    else
    {
        new->pipeline[i].redirect[*j].file = NULL;
        line->lexer_err = -8; // fini par << sans delimiteur
    }
    // if (!new->pipeline[i].redirect[*j].file)
    //     return (free(new->pipeline[i].redirect[*j].redir), 1);
    new->pipeline[i].redirect[*j].order = *j;
    (*j)++;
    temp = temp->next;
    return (0);
}

int    parse_assignment(t_line *line, t_expr *new, t_token *temp, int i, int *j)
{
    (void)line;
    new->pipeline[i].assign[*j].name = dup_assign_name(temp->s);
    if (!new->pipeline[i].assign[*j].name)
        return (EX_GEN);
    new->pipeline[i].assign[*j].value = dup_assign_value(temp->s);
    if (!new->pipeline[i].assign[*j].value) 
        return (free(new->pipeline[i].assign[*j].name), 1);
    (*j)++;
    return (0);
}