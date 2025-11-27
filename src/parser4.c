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
	t_expr1	exp;

	exp.new = init_new_expr(line, op_ctrl);
	if (!exp.new)
		return (NULL);
	exp.temp = line->tokens;
	exp.i = 0;
	while (line->tokens && line->tokens->type != op_ctrl)
	{
		ft_bzero(exp.len, sizeof(exp.len));
		count_token(line->tokens, &exp.len, op_ctrl);
		exp.new->pipeline[exp.i] = init_pipeline(line, &exp.len);
		if (line->last_exit != 0)
			return (free_p(&exp.new->pipeline[exp.i]), free_e(exp.new), NULL);
		line->tokens = p_pipe(line, &exp);
		if (line->last_exit != 0)
			return (free_p(&exp.new->pipeline[exp.i]), free_e(exp.new), NULL);
		if (!line->tokens)
			break ;
		if (line->tokens->type == PIPE)
			line->tokens = line->tokens->next;
		else
			break ;
	}
	return (exp.new->next = NULL, line->tokens = exp.temp, exp.new);
}

size_t	sft_count_words(const char *s, char c)
{
	size_t	word;

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

void	init_parse1(t_parse1 *p)
{
	p->split = NULL;
	p->new_args = NULL;
	p->tempo = NULL;
	p->i = 0;
	p->word = 0;
	p->m = 0;
	p->l = 0;
}

void	normalize_spaces(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (is_whitespace(s[i]))
			s[i] = ' ';
		i++;
	}
}

int	alloc_new_args(t_expr1 *exp, t_token *temp, t_parse1 *p)
{
	p->word = sft_count_words(temp->s, ' ');
	if (p->word == 1)
		return (0);
	exp->len[0] += p->word - 1;
	p->word += exp->new->pipeline[exp->i].word_count;
	p->new_args = ft_calloc(p->word + 1, sizeof(char *));
	if (!p->new_args)
		return (-1);
	p->split = ft_split(temp->s, ' ');
	if (!p->split)
	{
		free(p->new_args);
		return (-1);
	}
	return (1);
}
