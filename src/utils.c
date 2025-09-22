/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:54 by nburgevi          #+#    #+#             */
/*   Updated: 2025/08/26 04:38:58 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_quote(int c)
{
	if (c == 39)
		return (c);
	if (c == 34)
		return (c);
	return (0);
}

int	is_whitespace(int c)
{
	if (c >= 9 && c <= 13)
		return (c);
	if (c == 32)
		return (c);
	return (0);
}

int	is_special(int c)
{
	if (c == '|')
		return (c);
	if (c == '&')
		return (c);
	if (c == '<')
		return (c);
	if (c == '>')
		return (c);
	return (0);
}

int	is_dollar(int c)
{
	if (c == '$')
		return (c);
	return (0);
}

int	is_equal(int c)
{
	if (c == '=')
		return (c);
	return (0);
}