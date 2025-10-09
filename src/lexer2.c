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

char    *lexer_simple_char(t_line *line, char *s, char *start, char *end)
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
    int     open;
    int     close;
    int     temp_last_exit;

    open = 1;
    close = 0;
    temp_last_exit = 0;
    while (*s && open != close)
    {
        s++;
        if (*s == '(')
            open++;
        if (*s == ')')
            close++;
    }
    if (*s == 0 && open != close)
        temp_last_exit = -3; // paranthese non fermee
    end = s;
    add_back(line, create_token(line, start, (end - start) + 1));
    if (line->last_exit != 0)
        return (NULL);
    return (line->last_exit = temp_last_exit, s);
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