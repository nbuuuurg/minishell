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

void    count_token(t_token *temp, int (*len)[3], t_token_type op_ctrl)
{
    t_token    *temp2;

    temp2 = temp;
    while(temp2 && temp2->type != PIPE && temp2->type != op_ctrl)
    {
        if (temp2->type == WORD && temp2->in_subshell == 0)
        {
            if (!temp2->next)
                (*len)[0]++;
            else if (temp2->next && temp2->next->type != REDIR_IN && temp2->next->type != REDIR_APPEND && temp2->next->type != REDIR_OUT && temp2->next->type != HEREDOC)
                (*len)[0]++;
        }
        if (temp2->type == REDIR_IN || temp2->type == REDIR_APPEND || temp2->type == REDIR_OUT || temp2->type == HEREDOC)
            (*len)[1]++;
        if (temp2->type == ASSIGNMENT)
            (*len)[2]++;
        temp2 = temp2->next;
    }
}

void    find_pipe_position(t_expr *new, t_token *temp, int i)
{
    if (temp)
    {
        if (new->pipe_count == 0)
            new->pipeline[i].position = NO_PIPE;
        if (new->pipe_count != 0)
        {
            if (i == 0)
                new->pipeline[i].position = FIRST;
            else if (i != 0 && i < new->pipe_count)
                new->pipeline[i].position = MIDDLE;
            else
                new->pipeline[i].position = LAST;
        }
    }
    else if (!temp)
    {
        if (new->pipe_count == 0)
            new->pipeline[i].position = NO_PIPE;
        else
            new->pipeline[i].position = LAST;
    }
}

char    *expanded_var(t_line *line, char *var)
{
    char    *expanded_var;
    char    *the_env;

    if (!var)
        return (NULL);
    if (ft_strncmp(var, "$$", 2) == 0)
    {
        expanded_var = ft_itoa(getpid());
        if (!expanded_var)
            return (free(var), NULL);
    }
    else if (ft_strncmp(var, "$?", 2) == 0)
    {
        expanded_var = ft_itoa(line->last_exit);
        if (!expanded_var)
            return (free(var), NULL);
    }
    else if (ft_strncmp(var,"$", 1) == 0)
    {
        expanded_var = ft_strdup("");
        if (!expanded_var)
            return (free(var), NULL);
    }
    else
    {
        //decouper
        the_env = getenv(var);
        if (!the_env)
        {
            expanded_var = ft_strdup("\0");
            if (!expanded_var)
                return (free(var), NULL);
        }
        else
        {
            expanded_var = ft_strdup(the_env);
            if (!expanded_var)
                return (free(var), NULL);
        }
    }
    free(var);
    return (expanded_var);

}

int    expanded_token(t_line *line, t_token *token, char *var, size_t start, size_t end)
{
    (void)line;
    (void)token;
    (void)var;
    (void)start;
    (void)end;

    char    *s;
    size_t  len_var;
    size_t  len_old_var;
    size_t  len_s;
    size_t  i;
    size_t  j;
    size_t  k;

    // print_token(line);
    if (*var == 0)
        len_var = 0;
    else
        len_var = ft_strlen(var);
    len_old_var = end - start + 1;
    len_s = ft_strlen(token->s) - len_old_var + len_var;
    if (len_s == 0)
    {

    }
    s = ft_calloc(len_s + 1, 1);
    if (!s)
        return (line->last_exit = EX_GEN, -1);
    i = 0;
    k = 0;
    while(i < len_s && len_s != 0)
    {
        if (i == start)
        {
            j = 0;
            k += len_old_var;
            if (len_var != 0)
            {
                while (j < len_var)
                    s[i++] = var[j++];
            }
            else
                s[i++] = token->s[k++];
        }
        else
                s[i++] = token->s[k++];
    }
    s[i] = 0;
    free(token->s);
    token->s = ft_strdup(s);
    if (!token->s)
        return (free(s), line->last_exit = EX_GEN, -1);
    free(s);
    return (start + len_var - 1);
}

char    *parse_quoted_token(t_line *line, t_token *token)
{
    char    *s;
    int     d_quote;
    int     s_quote;
    int     i;
    int     j;
    int     len;

    i = 0;
    j = 0;
    d_quote = 0;
    s_quote = 0;
    if (token->quoted == SINGLE || token->quoted == DOUBLE)
    {
        s = ft_calloc(ft_strlen(token->s) - 1, 1);
        if (!s)
            return (line->last_exit = EX_GEN, NULL);
        while (token->s[i])
        {
            if (is_quote(token->s[i]))
                i++;
            s[j++] = token->s[i++];
        }
    }
    else
    {
        len = 0;
        while (token->s[i])
        {
            if (token->s[i] == 39 && d_quote == 0)
            {
                len++;
                s_quote++;
                if (s_quote == 2)
                    s_quote = 0;
            }
            else if (token->s[i] == 34 && s_quote == 0)
            {
                len++;
                d_quote++;
                if (d_quote == 2)
                    d_quote = 0;     
            }
            i++;
        }
        s = ft_calloc(ft_strlen(token->s) - len + 1, 1);
        if (!s)
            return (line->last_exit = EX_GEN, NULL);
        i = 0;
        j = 0;
        d_quote = 0;
        s_quote = 0;
        while (token->s[i])
        {
            if (token->s[i] == 34 && s_quote == 0)
            {
                d_quote++;
                if (d_quote == 2)
                    d_quote = 0;
            }
            else if (token->s[i] == 39 && d_quote == 0)
            {
                s_quote++;
                if (s_quote == 2)
                    s_quote = 0;
            }
            else
                s[j++] = token->s[i];
            i++;
        }
    }
    (void)line;
    free(token->s);
    token->s = ft_strdup(s);
    if (!token->s)
        return (free(s), line->last_exit = EX_GEN, NULL);
    return (token->s);
}

char	*parse_expand(t_line *line, t_token *token)
{
    int     i;
    int     start;
    int     end;
    int     len;
    int     s_quote;
    int     d_quote;
    char    *var;

    i = 0;
    start = 0;
    end = 0;
    s_quote = 0;
    d_quote = 0;
    // printf("token->s : %s\n", token->s);
    // print_token(line);
    while (token->s[i])
    {
        start = i;
        end = i;
        len = 0;
        var = NULL;
        if (token->s[i] == 34)
        {
            if (d_quote == 0 && s_quote == 0)
                d_quote = 1;
            else if (d_quote == 1)
                d_quote = 0;
        }
        if (token->s[i] == 39)
        {
            if (s_quote == 0 && d_quote == 0)
                s_quote = 1;
            else if (s_quote == 1)
                s_quote = 0;
        }
        if (token->s[i] == '$' && s_quote == 0)
        {
            if (token->s[i + 1])
            {
                if (token->s[i + 1] == '?')
                {
                    var = ft_strdup("$?");
                    if (!var)
                        return (line->last_exit = EX_GEN, NULL);
                    end++;
                }
                else if (token->s[i + 1] == '$')
                {
                    var = ft_strdup("$$");
                    if (!var)
                        return (line->last_exit = EX_GEN, NULL);
                    i++;
                    end++;
                }
                else
                {
                    while (token->s[i + 1] && ft_isalnum(token->s[i + 1]))
                    {
                        len++;
                        i++;
                    }
                    if (len != 0)
                    {
                        var = ft_calloc(len + 1, 1);
                        if (!var)
                            return (line->last_exit = EX_GEN, NULL);
                        ft_memcpy(var, &token->s[i + 1 - len], len);
                    }
                    end = i;
                }
            }
            else
            {
                var = ft_strdup("$");
                if (!var)
                    return (line->last_exit = EX_GEN, NULL);
            }
            var = expanded_var(line, var);
            if (!var)
                return (line->last_exit = EX_GEN, NULL);
            i = expanded_token(line, token, var, start, end);
            if (i == -1 && line->last_exit != 0)
                return (NULL);
            free(var);
        }
        i++;
    }
    (void)line;
	return (line->last_exit = 0, token->s);
}