/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:02:38 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 09:02:39 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*new_subinput(t_line *line, t_token *token)
{
	t_sub1	d;

	(void)line;
	if (!init_sub1(&d, token))
		return (NULL);
	fill_subinput(&d, token);
	add_previous_pipes(line, &d, token);
	if (line->last_exit != 0)
		return (NULL);
	add_next_pipes(&d, token);
	return (d.subinput);
}

int	init_sub1(t_sub1 *d, t_token *token)
{
	d->len = ft_strlen(token->s);
	if (d->len < 2)
		return (0);
	d->subinput = ft_calloc(d->len - 1, 1);
	if (!d->subinput)
		return (0);
	d->i = 0;
	d->j = 0;
	d->subshell = 0;
	d->subshell2 = 0;
	d->temp = NULL;
	d->ttemp = NULL;
	return (1);
}

void	fill_subinput(t_sub1 *d, t_token *token)
{
	while (token->s[d->i] && d->i < d->len - 1)
	{
		if ((d->subshell == 0 && token->s[d->i] == '(')
			|| (d->subshell2 == 0
				&& token->s[d->len - d->i - 1] == ')'))
		{
			if (d->subshell == 0 && token->s[d->i] == '(')
				d->subshell++;
			if (d->subshell2 == 0
				&& token->s[d->len - d->i - 1] == ')')
				d->subshell2++;
			d->i++;
		}
		if (token->s[d->i] && d->i < d->len - 1)
			d->subinput[d->j++] = token->s[d->i++];
	}
}

void	add_previous_pipes(t_line *line, t_sub1 *d, t_token *token)
{
	char	*old_s;

	d->ttemp = token;
	if (!d->ttemp->previous)
		return ;
	if (d->ttemp->previous->type != PIPE)
		return ;
	while (d->ttemp->previous
		&& d->ttemp->previous->type != AND
		&& d->ttemp->previous->type != OR)
	{
		old_s = d->ttemp->previous->s;
		d->temp = ft_strjoin(d->ttemp->previous->s, " ");
		if (!d->temp)
			return (line->last_exit == 1);
		free(old_s);
		d->ttemp->previous->s = d->temp;
		d->temp = ft_strjoin(d->ttemp->previous->s, d->subinput);
		if (!d->temp)
			return (line->last_exit == 1);
		free(d->subinput);
		d->subinput = d->temp;
		d->ttemp = d->ttemp->previous;
	}
}

void	add_next_pipes(t_sub1 *d, t_token *token)
{
	d->ttemp = token;
	if (!d->ttemp->next)
		return ;
	if (d->ttemp->next->type != PIPE)
		return ;
	while (d->ttemp->next
		&& d->ttemp->next->type != AND
		&& d->ttemp->next->type != OR)
	{
		d->temp = ft_strjoin(d->subinput, " ");
		free(d->subinput);
		d->subinput = d->temp;
		d->temp = ft_strjoin(d->subinput, d->ttemp->next->s);
		free(d->subinput);
		d->subinput = d->temp;
		d->ttemp = d->ttemp->next;
	}
}
