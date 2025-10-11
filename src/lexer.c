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
    char    *start = NULL;
    char    *end = NULL;

    s = line->input;
    start = s;
    while (*s)
    {
        while(*s && is_whitespace(*s))
            s++;
        start = s;
        while (*s)
        {
            if (*s && is_special(*s))
            {
                s = lexer_special_char(line, s, start, end);
                if (line->last_exit != 0)
                    return (line->last_exit);
                start = s + 1;
            }
            if (*s && is_whitespace(*s))
            {
                s = lexer_simple_char(line, s, start, end);
                if (line->last_exit != 0)
                    return (line->last_exit);
                start = s + 1;
            }
            if (*s && is_quote(*s))
            {
                s = lexer_quoted_char(line, s, start, end);
                if (line->last_exit != 0)
                    return (line->last_exit);
                start = s + 1;
                s--;
            }
            if (*s && is_subshell(*s))
            {
                if (is_subshell(*s) == ')')
                    return (-2); // commence par "("
                else
                {
                    s = lexer_subchell_char(line, s, start, end);
                    if (line->last_exit != 0)
                        return (line->last_exit);
                    start = s + 1;
                }
            }
            s++;
            end = s;
        }
    }
    if (*s == 0)
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

    temp = line->tokens;
    temp2 = line->tokens;
    expr = NULL;
    new = NULL;
    i = 0;
    while (temp)
    {
        expr = line->exprs;
        // if (temp->in_subshell != 0)
        //     line->last_exit = lexer_subshell_expr(line, temp, new, expr, i);
        if (temp->type == AND || temp->type == OR)
            line->last_exit = lexer_split_expr(line, temp, new, expr, i);
        if (line->last_exit != 0)
            return (line->last_exit);
        temp = temp->next;
        if (!temp)
            line->last_exit = lexer_single_expr(line, new, expr);
        i++;
        if (line->last_exit != 0)
            return (line->last_exit);
    }
    line->tokens = temp2;
    return (0);
}

int    lexer_subshell_expr(t_line *line, t_token *temp, t_expr *new, t_expr *expr, int i)
{
    (void)line; (void)temp; (void)new; (void)expr; (void)i;
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
    return (0);
}