/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:05:36 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/26 17:27:14 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_str(char *arg)
{
	char	*r;
	int		i;

	if (!arg)
	{
		ft_putstr_fd("(null)", 1);
		return (i = 6);
	}
	r = ft_strdup(arg);
	if (r == NULL)
		return (0);
	i = 0;
	while (r[i])
	{
		ft_putchar_fd(r[i], 1);
		i++;
	}
	free(r);
	return (i);
}
