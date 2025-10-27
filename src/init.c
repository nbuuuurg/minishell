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

void    init_minishell(t_line *line, char **envp, int start_flag)
{
    init_line(line, envp, start_flag);
    if (line->last_exit != 0)
        return ;
    if (line->len == 0)
        return ;
    line->last_exit = lexer_input(line);
    // printf("last_exit : %d\n", line->last_exit);
    // printf("lexer_err : %d || token->s : %s\n", line->lexer_err, line->tokens->s);
}

int    init_clean_input(t_line *line)
{
    size_t  i;
    size_t  j;

    if (line->len == 0 || line->len == 1)
        return (0);
    j = len_whitespace(line);
    line->clean = ft_calloc(line->len - j + 1, 1);
    if (!line->clean)
        return (EX_GEN);
    i = 0;
    while (line->input[i] && is_whitespace(line->input[i]))
        i++;
    ft_memcpy(line->clean, &line->input[i], line->len - j);
    free(line->input);
    line->input = line->clean;
    // printf("$%s$\n", line->clean);
    return (0);
}

void    init_line(t_line *line, char **envp, int start_flag)
{
    line->tokens = NULL;
    line->exprs = NULL;
    line->cmd = NULL;
    line->num_expr = 0;
    line->lexer_err = 0;
    line->heredoc_flag = 0;
    line->path = get_path(envp);
    line->len = ft_strlen(line->input);
    if (envp && start_flag == 0)
        line->envp = envp;
    line->last_exit = init_clean_input(line);
    if (line->last_exit != 0)
        return ;
}

void	init_token(t_token *token, int multiple_quote, int quote, int i)
{
	token->s[i] = 0;
	token->quoted = def_quote(multiple_quote, quote);
	token->previous = NULL;
	token->next = NULL;
	token = token_type(token);
	token = has_expand(token);
}

t_expr  *init_new_expr(t_line *line, t_token_type op_ctrl)
{
    t_expr      *new;
    t_token     *temp;
    int         pipe;
    int         i;

    new = ft_calloc(1, sizeof(t_expr));
    if (!new)
        return (NULL);
    new->op_after = op_ctrl;
    temp = line->tokens;
    pipe = 0;
    i = 0;
    while(temp && temp->type != op_ctrl)
    {
        if (temp->type == PIPE)
            pipe++;
        i++;
        temp = temp->next;
    }
    new->pipe_count = pipe;
    new->has_subshell = 0;
    new->pipeline = ft_calloc(new->pipe_count + 1, sizeof(t_pipeline));
    if (!new->pipeline)
        return (free(new), NULL);
    return (new);
}
