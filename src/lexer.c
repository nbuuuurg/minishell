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

char    *lexer_special_char(t_line *line, char *s, char *start, char *end)
{
    if (s == line->input)
    {
        start = s;
        end = start;
        while (*end && *end == is_special(*s) && end != s + 1)
            end++;
        add_back(line, create_token(line, start, (end - start) + 1));
        if (*(end + 1) == 0)
            return (line->last_exit = -7, NULL);
        return (line->last_exit = 0, s = end);
    }
    if (*(s - 1) && !is_whitespace(*(s - 1)) && !ft_isdigit(*(s - 1)))
    {
        end = s - 1;
        add_back(line, create_token(line, start, (end - start) + 1));
        if (line->last_exit != 0)
            return (NULL);
    }
    if (*(s + 1))
    {
        if (*(s + 1) == is_special(*s))
        {
            start = s;
            if (*(start - 1) && ft_isdigit(*(start - 1)) && (is_special(*s) == '>' ||is_special(*s) == '<'))
            {
                start--;
                while (*(start - 1) && ft_isdigit(*(start - 1)))
                    start--;
                if ((*(start - 1) && !is_whitespace(*(start - 1))) && !is_special(*(start - 1)))
                {
                    if (*(start - 1) && !is_quote(*(start - 1)) && !is_special(*(start - 1)))
                    {
                        end = s - 1;
                        add_back(line, create_token(line, start, (end - start) + 1));
                        if (line->last_exit != 0)
                            return (NULL);
                    }
                    start = s;
                }
            }
            end = s + 1;
            add_back(line, create_token(line, start, (end - start) + 1));
            if (line->last_exit != 0)
                return (NULL);
            if (*(s + 2) == 0)
                return (line->last_exit = -5, NULL); // fini par double special
            s++;
        }
        else
        {
            start = s;
            if (*(start - 1) && ft_isdigit(*(start - 1)) && (is_special(*s) == '>' ||is_special(*s) == '<'))
            {
                start--;
                while (*(start - 1) && ft_isdigit(*(start - 1)))
                    start--;
                if ((*(start - 1) && !is_whitespace(*(start - 1))) &&!is_special(*(start - 1)))
                {
                    if (*(start - 1) && !is_quote(*(start - 1)) && !is_special(*(start - 1)))
                    {
                        end = s - 1;
                        add_back(line, create_token(line, start, (end - start) + 1));
                        if (line->last_exit != 0)
                            return (NULL);
                    }
                    start = s;
                }
            }
            end = s;
            add_back(line, create_token(line, start, (end - start) + 1));
            if (line->last_exit != 0)
                return (NULL);
        }
    }
    else
        return (line->last_exit = -6, NULL); // fini par simple specia
    return (line->last_exit = 0, s);
}

char    *lexer_ascii_char(t_line *line, char *s, char *start, char *end)
{
    if (line->len != 1 && *(s - 1) && !is_special(*(s - 1)))
    {
        end = s - 1;
        add_back(line, create_token(line, start, (end - start) + 1));
        if (line->last_exit != 0)
            return (NULL);
    }
    return (s);
}

char    *lexer_quoted_char(t_line *line, char *s, char *start, char *end)
{
    int quote;

    quote = is_quote(*s);
    s++;
    while (*s && *s != quote)
        s++;
    if (*s == 0)
        return (line->last_exit = -1, NULL); // quote non terminee
    while (*s && !is_whitespace(*s) && !is_special(*s))
    {
        s++;
        if (*s && is_quote(*s))
        {
            quote = is_quote(*s);
            s++;
            while (*s && *s != quote)
                s++;
            if (*s == 0)
                return (line->last_exit = -1, NULL); // quote non terminee 
        }
    }
    end = s;
    add_back(line, create_quoted_token(line, start, (end - start) + 1 , quote));
    if (line->last_exit != 0)
        return (NULL);
    return (s);
}

char    *lexer_subchell_char(t_line *line, char *s, char *start, char *end)
{
    int     quote;

    if (line->len != 1 && *(s - 1) && !is_whitespace(*(s - 1)))
    {
        end = s - 1;
        add_back(line, create_token(line, start, (end - start) + 1));
        if (line->last_exit != 0)
            return (NULL);
    }
    quote = ')';
    while (*s && *s != quote)
        s++;
    if (*s == 0)
        return (line->last_exit = -3, NULL); // paranthese non fermee
    end = s;
    add_back(line, create_token(line, start, (end - start) + 1));
    if (line->last_exit != 0)
        return (NULL);
    return (s);
}

char    *lexer_last_char(t_line *line, char *s, char *start, char *end)
{
    if (*(s - 1))
    {
        if (!is_whitespace(*(s - 1)) && !is_quote(*(s - 1)) && !is_subshell(*(s - 1)))
        {
            end = s;
            add_back(line, create_token(line, start, (end - start) + 1));
            if (line->last_exit != 0)
                return (NULL);
        }
    }
    return (s);
}

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
            if (is_special(*s))
            {
                s = lexer_special_char(line, s, start, end);
                if (line->last_exit != 0)
                    return (line->last_exit);
                start = s + 1;
            }
            if (is_whitespace(*s))
            {
                s = lexer_ascii_char(line, s, start, end);
                if (line->last_exit != 0)
                    return (line->last_exit);
                start = s + 1;
            }
            if (is_quote(*s))
            {
                s = lexer_quoted_char(line, s, start, end);
                if (line->last_exit != 0)
                    return (line->last_exit);
                start = s + 1;
                s--;
            }
            if (is_subshell(*s))
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

int    lexer_split_expr(t_line *line, t_token *temp, t_expr *new, t_expr *expr, int i)
{
    if (temp->type == AND || temp->type == OR)
    {
        if (i == 0 || !temp->next)
            return (-4); // commence ou fini par || ou &&
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