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

void	free_new_args_failure(t_parse1 *p)
{
	while (--p->m >= 0)
		free(p->new_args[p->m]);
	free(p->new_args);
	if (p->split)
		free_split(p->split);
}

int	dup_split_into_new_args(t_parse1 *p)
{
	p->l = 0;
	while (p->split[p->l])
	{
		p->new_args[p->m] = ft_strdup(p->split[p->l]);
		if (!p->new_args[p->m])
			return (0);
		p->m++;
		p->l++;
	}
	return (1);
}

int	dup_arg_into_new_args(t_expr1 *exp, t_parse1 *p)
{
	p->new_args[p->m] = ft_strdup(exp->new->pipeline[exp->i].args[p->i]);
	if (!p->new_args[p->m])
		return (0);
	p->m++;
	return (1);
}

char	**d_w_t(t_line *line, t_expr1 *exp, t_token *temp)
{
	t_parse1	p;
	int			ret;

	(void)line;
	init_parse1(&p);
	if (!temp || !exp->new->pipeline[exp->i].args)
		return (exp->new->pipeline[exp->i].args);
	normalize_spaces(temp->s);
	ret = alloc_new_args(exp, temp, &p);
	if (ret <= 0)
	{
		if (ret == 0)
			return (exp->new->pipeline[exp->i].args);
		return (NULL);
	}
	if (!fill_new_args(exp, &p))
	{
		free_new_args_failure(&p);
		return (NULL);
	}
	exp->new->pipeline[exp->i].word_count = p.word;
	if (exp->new->pipeline[exp->i].args)
		free_split(exp->new->pipeline[exp->i].args);
	free_split(p.split);
	return (p.new_args);
}

int	fill_new_args(t_expr1 *exp, t_parse1 *p)
{
	p->tempo = exp->new->pipeline[exp->i].args;
	p->i = 0;
	p->m = 0;
	while (p->tempo[p->i])
	{
		if (!p->tempo[p->i + 1])
		{
			if (!dup_split_into_new_args(p))
				return (0);
		}
		else if (!dup_arg_into_new_args(exp, p))
			return (0);
		p->i++;
	}
	return (1);
}
