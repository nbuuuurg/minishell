/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:31 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:25:26 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expanded_var(t_line *line, char *var)
{
	char	*expanded_var;
	char	*the_env;

	if (!var)
		return (NULL);
	if (ft_strncmp(var, "$$", 3) == 0 && ft_strlen(var) == 2)
		expanded_var = ft_itoa(getpid());
	else if (ft_strncmp(var, "$?", 3) == 0 && ft_strlen(var) == 2)
		expanded_var = ft_itoa(line->prev_exit);
	else if (ft_strncmp(var, "$", 2) == 0 && ft_strlen(var) == 1)
		expanded_var = ft_strdup("$");
	else
	{
		the_env = find_env_var(line, var);
		if (!the_env)
			expanded_var = ft_strdup("");
		else
			expanded_var = ft_strdup(the_env);
	}
	free(var);
	if (!expanded_var)
		return (NULL);
	return (expanded_var);
}

int	expand_t(t_line *line, t_token *token, t_parse5 *p2)
{
	t_parse3	p;

	if (init_expand_t(&p, p2, token) == 1)
		return (line->last_exit = EX_GEN, -1);
	while (p.i < p.len_s && p.len_s != 0)
	{
		if ((int)p.i == p2->start)
		{
			p.k += p.len_old_var;
			if (p.len_var != 0)
			{
				while (p.j < p.len_var)
					p.s[p.i++] = p2->var[p.j++];
			}
			else
				p.s[p.i++] = token->s[p.k++];
		}
		else
			p.s[p.i++] = token->s[p.k++];
	}
	free(token->s);
	token->s = ft_strdup(p.s);
	if (!token->s)
		return (free(p.s), line->last_exit = EX_GEN, -1);
	return (free(p.s), p2->start + p.len_var - 1);
}

char	*parse_expand(t_line *line, t_token *token)
{
	t_parse5	p;

	init_parse5(&p);
	while (token->s[p.i])
	{
		init_parse5_loop(&p);
		if (token->s[p.i] == 34 || token->s[p.i] == 39)
			define_quotes(&p, token);
		else if (token->s[p.i] == '$' && p.s_quote == 0)
		{
			p.var = parse_expand_helper(&p, line, token);
			if (!p.var)
				return (line->last_exit = EX_GEN, NULL);
			p.var = expanded_var(line, p.var);
			if (!p.var)
				return (line->last_exit = EX_GEN, NULL);
			p.i = expand_t(line, token, &p);
			if (p.i == -1 && line->last_exit != 0)
				return (NULL);
			free(p.var);
		}
		p.i++;
	}
	return (line->last_exit = 0, token->s);
}
