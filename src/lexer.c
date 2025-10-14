/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:24 by nburgevi          #+#    #+#             */
/*   Updated: 2025/09/03 13:11:40 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int    lexer_input(t_line *line)
{
    char    *s;
    char    *start;
    char    *end;

    s = line->input;
    start = s;
    end = NULL;
    while (*s)
    {
        if (*s && is_something(*s))
        {
            s = lexer_input_something(line, s, start, end);
            if (line->last_exit != 0)
                return (line->last_exit);
            start = s + 1;
        }
        s++;
        end = s;
    }
    if (*s == 0 && *start != 0)
    {
        s = lexer_last_char(line, s, start, end);
        if (line->last_exit != 0)
            return (line->last_exit);
    }
    return (lexer_token(line));
}

int    lexer_token(t_line *line)
{
    int     i;
    t_token *temp;
    t_token *temp2;
    t_expr  *expr;
    t_expr  *new;
    int     flag;

    temp = line->tokens;
    temp2 = line->tokens;
    expr = NULL;
    new = NULL;
    i = 0;
    flag = 0;
    temp = line->tokens;
    while (temp)
    {
        // printf("last_exit : %d\n", line->last_exit);
        // printf("lexer_err : %d || token->s : %s\n", line->lexer_err, line->tokens->s);
        expr = line->exprs;
        if (temp->in_subshell > 0)
        {
            if (line->lexer_err == -2)
                return (line->last_exit); // parenthese non ferme
            line->last_exit = init_subshell(line, temp);
            if (line->last_exit != 0)
                return (line->last_exit);
            flag++;
        }
        if (temp->type == AND || temp->type == OR)
            line->last_exit = lexer_split_expr(line, temp, new, expr, i);
        if (line->last_exit != 0)
            return (line->last_exit);
        temp = temp->next;
        if (!temp && flag == 0)
            line->last_exit = lexer_single_expr(line, new, expr);
        i++;
        if (line->last_exit != 0)
            return (line->last_exit);
        flag = 0;
    }
    line->tokens = temp2;
    return (0);
}

int    lexer_split_expr(t_line *line, t_token *temp, t_expr *new, t_expr *expr, int i)
{
    if (temp->type == AND || temp->type == OR)
    {
        if (i == 0 || !temp->next)
            return (line->lexer_err = -4, 0); // commence ou fini par || ou &&
        new = parse_new_expr(line, temp->type);
        if (!new)
            return (EX_GEN);
        if (!line->exprs)
            line->exprs = new;
        else
        {
            while (expr->next)
                expr = expr->next;
            expr->next = new;
        }
        line->tokens = temp->next;
    }
    exec_exprs(new, line->path, line->envp, line);
    return (0);
}

int    lexer_single_expr(t_line *line, t_expr *new, t_expr *expr)
{
    new = parse_new_expr(line, 9);
    if (!new)
        return (line->last_exit = EX_GEN);
    if (!line->exprs)
        line->exprs = new;
    else
    {
        while (expr->next)
            expr = expr->next;
        expr->next = new;
    }
    exec_exprs(new, line->path, line->envp, line);
    return (0);
}