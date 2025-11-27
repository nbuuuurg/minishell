/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 01:36:29 by adeflers          #+#    #+#             */
/*   Updated: 2025/11/05 09:10:17 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	match_tab_advance(char **p, char c, int *ok)
{
	char		*ptr;
	char		start;
	char		end;

	ptr = *p;
	if (*(ptr + 1) == '-' && *(ptr + 2) && *(ptr + 2) != ']')
	{
		start = *ptr;
		end = *(ptr + 2);
		if (start <= c && c <= end)
			*ok = 1;
		ptr += 3;
	}
	else
	{
		if (*ptr == c)
			*ok = 1;
		ptr++;
	}
	*p = ptr;
}

int	match_tab_class(char **pattern, char c, int *neg_out)
{
	char		*p;
	int			neg;
	int			ok;

	p = *pattern;
	neg = 0;
	ok = 0;
	if (*p == '!' || *p == '^')
	{
		neg = 1;
		p++;
	}
	while (*p && *p != ']')
		match_tab_advance(&p, c, &ok);
	if (*p == ']')
		p++;
	*pattern = p;
	*neg_out = neg;
	return (ok);
}

int	match_tab(char **pattern, char c)
{
	int	neg;
	int	ok;

	ok = match_tab_class(pattern, c, &neg);
	if (neg)
		return (!ok);
	return (ok);
}
