/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:31 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:19:54 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_expr	*parse_new_expr(t_line *line, t_token_type op_ctrl)
{
	int		i;
	int		len[3];
	t_expr	*new;
	t_token	*temp;

	new = init_new_expr(line, op_ctrl);
	if (!new)
		return (NULL);
	temp = line->tokens;
	i = 0;
	while (line->tokens && line->tokens->type != op_ctrl)
	{
		ft_bzero(len, sizeof(len));
		count_token(line->tokens, &len, op_ctrl);
		new->pipeline[i] = init_pipeline(line, &len);
		if (line->last_exit != 0)
			return (free_pipeline(&new->pipeline[i]), free_exprs(new), NULL);
		line->tokens = p_pipe(line, line->tokens, new, &len, &i);
		if (line->last_exit != 0)
			return (free_pipeline(&new->pipeline[i]), free_exprs(new), NULL);
		if (!line->tokens)
			break ;
		if (line->tokens->type == PIPE)
			line->tokens = line->tokens->next;
		else
			break ;
	}
	new->next = NULL;
	line->tokens = temp;
	// while(temp)
	// {
	// 	printf("temp->s : %s\n", temp->s);
	// 	temp = temp->next;
	// }
	return (new);
}

static size_t	sft_count_words(const char *s, char c)
{
	size_t	word;

	/* if (!s) */
	/* 	return (0); */
	word = 1;
	while (*s && *s == c)
		s++;
	if (*s == 0)
		return (0);
	while (*(s + 1))
	{
		if (*s == c && *(s + 1) != 0 && *(s + 1) != c)
			word++;
		s++;
	}
	return (word);
}


char	**deal_with_that(t_line *line, t_token *temp, t_expr *new, int *w, int k)
{
	char	**split = 0;
	char	**new_args = 0;
	char	**tempo = 0;
	int		i = 0;
	int		word = 0;

	(void)line;
	if (!temp || !new->pipeline[k].args)
		return (new->pipeline[k].args);
	while(temp->s[i])
	{
		if (is_whitespace(temp->s[i]))
			temp->s[i] = ' ';
		i++;
	}
	i = 0;
	word = sft_count_words(temp->s, ' ');
	if (word == 1)
		return (new->pipeline[k].args);
	*w += word - 1; // met len[0] a jour
	word += new->pipeline[k].word_count;
	new_args = ft_calloc(word + 1, sizeof(char *));
	if (!new_args)
		return (NULL);
	split = ft_split(temp->s, ' ');
	if (!split)
		return (free(new_args), NULL);
	i = 0;
	tempo = new->pipeline[k].args;
	int m = 0;
	while(tempo[i])
	{
		if (!tempo[i + 1])
		{
			while(split[m])
			{
				new_args[m] = ft_strdup(split[m]);
				if (!new_args[m])
				{
					while (--m >= 0)
						free(new_args[m]);
					free(new_args);
					free_split(split);
					return (NULL);
				}
				m++;
			}
		}
		else
		{
			new_args[m] = ft_strdup(new->pipeline[k].args[i]);
			if (!new_args[m])
			{
				while(--m >= 0)
					free(new_args[m]);
				free(new_args);
				free_split(split);
				return (NULL);
			}
			m++;
		}
		i++;
	}
	new->pipeline[k].word_count = word;
	if (new->pipeline[k].args)
		free_split(new->pipeline[k].args);
	free_split(split);
	return (new_args);

}

t_token	*p_pipe(t_line *line, t_token *temp, t_expr *new, int (*len)[3], int *i)
{
	while (temp && temp->type != PIPE && temp->type != new->op_after)
	{
		if (temp->in_subshell != 0)
		{
			new->has_subshell = temp->in_subshell;
			line->last_exit = 0;
		}
		else if (temp->type == WORD)
		{
			line->last_exit = parse_word(line, new, temp, *i, &(*len)[0]);
			if ((sft_count_words(temp->s, ' ') > 1 || sft_count_words(temp->s, '\t') > 1) && temp->has_expand > 0 && temp->quoted == NO_QUOTE)
			{
				new->pipeline[*i].args = deal_with_that(line, temp, new, &(*len)[0], *i);
				if (!new->pipeline[*i].args)
					return (line->last_exit = 1, NULL);
			}
		}
		else if (temp->type == REDIR_IN || temp->type == REDIR_APPEND
			|| temp->type == REDIR_OUT || temp->type == HEREDOC)
			line->last_exit = parse_redir(line, new, temp, *i, &(*len)[1]);
		else if (temp->type == ASSIGNMENT)
			line->last_exit = parse_assignment(line, new, temp, *i, &(*len)[2]);
		if (line->last_exit != 0)
			return (NULL);
		if (line->heredoc_flag == 1)
			break ;
		temp = temp->next;
		find_pipe_position(new, temp, *i);
	}
	(*i)++;
	return (temp);
}

int	parse_word(t_line *line, t_expr *new, t_token *temp, int i, int *j)
{
	if (temp->previous
		&& (temp->previous->type == REDIR_IN
			|| temp->previous->type == REDIR_APPEND
			|| temp->previous->type == REDIR_OUT
			|| temp->previous->type == HEREDOC))
		return (0);
	if (temp->has_expand != 0)
	{
		temp->s = parse_expand(line, temp);
		// si c est NULL voir que passo 
		if (line->last_exit == EX_GEN)
			return (EX_GEN);
	}
	if (temp->quoted != NO_QUOTE)
		temp->s = parse_quoted_token(line, temp);
	if (line->last_exit == EX_GEN)
		return (EX_GEN);
	else
		new->pipeline[i].args[*j] = ft_strdup(temp->s);
	if (!new->pipeline[i].args[*j])
		return (EX_GEN);
	(*j)++;
	return (0);
}

int	parse_redir(t_line *line, t_expr *new, t_token *temp, int i, int *j)
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
		if (temp->next && temp->next->type == WORD
			&& temp->next->in_subshell == 0)
			new->pipeline[i].redirect[*j].hd_fd = hd_c(temp->next->s, line);
		else
			new->pipeline[i].redirect[*j].hd_fd = -1;
	}
	else if (temp->type == REDIR_APPEND)
		new->pipeline[i].redirect[*j].redir = ft_strdup(">>");
	else
		new->pipeline[i].redirect[*j].redir = ft_strdup(">");
	if (!new->pipeline[i].redirect[*j].redir)
		return (EX_GEN);
	if (temp->next && temp->next->type == WORD && temp->next->in_subshell == 0)
	{
		if (temp->next->has_expand != 0)
		{
			temp->next->s = parse_expand(line, temp->next);
			if (line->last_exit == EX_GEN)
				return (EX_GEN);
		}
		if (temp->next->quoted != NO_QUOTE)
			temp->next->s = parse_quoted_token(line, temp->next);
		if (line->last_exit == EX_GEN)
			return (EX_GEN);
		new->pipeline[i].redirect[*j].file = ft_strdup(temp->next->s);
	}
	else
	{
		new->pipeline[i].redirect[*j].file = NULL;
		line->lexer_err = -5;
	}
	new->pipeline[i].redirect[*j].order = *j;
	(*j)++;
	temp = temp->next;
	return (0);
}

int	parse_assignment(t_line *line, t_expr *new, t_token *temp, int i, int *j)
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
