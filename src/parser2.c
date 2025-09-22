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

void  find_pipe_position(t_expr *new, t_token *temp, int i)
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

char    *parse_quoted_token(t_line *line, t_token *token)
{
    (void)line;
    return (token->s);
}

char	*parse_expand(t_line *line, t_token *token)
{
    int     i;
    int     j;
    int     len;
    int     single_quote;
    char    **var;

    i = 0;
    j = 0;
    single_quote = 0;
    while (token->s[i])
    {
        if (token->s[i] == '$')
            j++;
        i++;
    }
    var = ft_calloc(j, 8);
    if (!var)
        exit(1); // clean exit - malloc fail
    j = 0;
    i = 0;
    while (token->s[i])
    {
        len = 0;
        if (token->s[i] == 39)
        {
            if (single_quote == 0)
                single_quote = 1;
            else
                single_quote = 0;
        }
        if (token->s[i] == '$' && single_quote == 0)
        {
            if (token->s[i - 1] && token->s[i - 1] == 92) // '\'
                i++;
            else if (token->s[i + 1])
            {
                if (token->s[i + 1] == '?')
                {
                    var[j] = ft_strdup("$?");
                    if (!var[j])
                        exit(1); // clean exit - malloc fail
                }
                else
                {
                    i++;
                    while (token->s[i] && ft_isalnum(token->s[i]))
                    {
                        len++;
                        i++;
                    }
                    printf("len = %d\n", len);
                    var[j] = ft_calloc(len + 1, 1);
                    if (!var[j])
                        exit(1); // clean exit - malloc fail
                    ft_strlcpy(var[j], &token->s[i - len], len + 1);
                }
                j++;
            }
        }
        i++;
    }
    printf("j = %d\n", j);
    int k = -1;
    while (++k < j)
        printf("var = %s\n", var[k]);
    (void)line;
    // essayer d'acceder a la var avec getenv, si ca ne marche pas remplacer par \n
    // si $? -> line->last_exit
    //remplacer ce qui doit etre remplacer dans token par var avec un realloc de memoire
	return (token->s);
}