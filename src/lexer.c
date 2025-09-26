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
    if (*(s - 1) && !is_whitespace(*(s - 1)) && !ft_isdigit(*(s - 1)))
    {
        end = s - 1;
        add_back(line, create_token(line, start, (end - start) + 1));
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
                    }
                    start = s;
                    // printf("s = %c | start = %c | end = %c",*s, *start, *end);
                }
            }
            end = s + 1;
            add_back(line, create_token(line, start, (end - start) + 1));
            if (*(s + 2) == 0)
            {
                printf("error d");
                exit(1); // clean exit - nothing after double special
            }
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
                // printf("s = %c | start = %c | end = %c\n",*s, *start, *end);
                if ((*(start - 1) && !is_whitespace(*(start - 1))) &&!is_special(*(start - 1)))
                {
                    if (*(start - 1) && !is_quote(*(start - 1)) && !is_special(*(start - 1)))
                    {
                        end = s - 1;
                        add_back(line, create_token(line, start, (end - start) + 1));
                    }
                    start = s;
                }
            }
            end = s;
            add_back(line, create_token(line, start, (end - start) + 1));
        }
    }
    else
    {
        printf("error s");
        exit(1) ; // clean exit - nothing after simple special
    }
    return (s);
}

void    lexer_input(t_line *line)
{
    char    *s;
    char    *start = NULL;
    char    *end = NULL;
    int    quote = 0;

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
                start = s + 1;
            }
            if (is_whitespace(*s))
            {
                if (*(s - 1) && !is_special(*(s - 1)))
                {
                    end = s - 1;
                    add_back(line, create_token(line, start, (end - start) + 1));
                }
                start = s + 1;
            }
            if (is_quote(*s))
            {
                quote = is_quote(*s);
                s++;
                while (*s && *s != quote)
                    s++;
                if (*s == 0)
                {
                    printf("error quote not ended");
                    exit(1); // free propre
                }
                while (*s && !is_whitespace(*s) && !is_special(*s))
                    s++;
                end = s;
                add_back(line, create_quoted_token(line, start, (end - start) + 1 , quote));
                start = s + 1;
                // if (is_special(*s))
                s--;
            }
            if (is_subshell(*s))
            {
                if (is_subshell(*s) == ')')
                {
                    printf("error syntaxe ')'\n");
                    exit(1); // clean exit - ')' detecte mais pas '('
                }
                else
                {
                    if (*(s - 1) && !is_whitespace(*(s - 1)))
                    {
                        end = s - 1;
                        // printf("start = %c et end = %c", *start, *end);
                        add_back(line, create_token(line, start, (end - start) + 1));
                    }
                    quote = ')';
                    while (*s && *s != quote)
                        s++;
                    if (*s == 0)
                    {
                        printf("error subshell not ended");
                        exit(1); // clean exit - subshell not ended
                    }
                    end = s;
                    add_back(line, create_token(line, start, (end - start) + 1));
                    start = s + 1;
                }
            }
            s++;
            end = s;
        }
    }
    if (*s == 0)
    {
        if (*(s - 1))
        {
            if (!is_whitespace(*(s - 1)) && !is_quote(*(s - 1)) && !is_subshell(*(s - 1)))
            {
                end = s;
                add_back(line, create_token(line, start, (end - start) + 1));
            }
        }
    }
    // print_token(line);
    lexer_token(line);
}

void    lexer_token(t_line *line)
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
            lexer_split_expr(line, temp, new, expr, i);
        temp = temp->next;
        if (!temp)
            lexer_single_expr(line, new, expr);
        i++;
    }
    line->tokens = temp2;
    // free tokens - not needed anymore
    print_expr(line);
}

void    lexer_split_expr(t_line *line, t_token *temp, t_expr *new, t_expr *expr, int i)
{
    if (temp->type == AND || temp->type == OR)
    {
        if (i == 0 || !temp->next)
            exit(1); // clean exit - ne peut pas commencer/finir par || ou &&
        new = parse_new_expr(line, temp->type);
        if (!new)
            exit(1); // clean exit - malloc fail
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
}

void    lexer_single_expr(t_line *line, t_expr *new, t_expr *expr)
{
    new = parse_new_expr(line, 9);
    if (!new)
        exit(1); // clean exit - malloc fail
    if (!line->exprs)
        line->exprs = new;
    else
    {
        while (expr->next)
            expr = expr->next;
        expr->next = new;
    }
}
