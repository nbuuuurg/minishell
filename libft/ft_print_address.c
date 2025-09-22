/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_address.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:02:24 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/26 17:26:46 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_address(void *arg)
{
	int		i;
	char	*address;

	if (!arg)
	{
		ft_putstr_fd("(nil)", 1);
		return (5);
	}
	address = ft_itoa_hex('x', (unsigned long)arg);
	i = 0;
	while (address[i])
		i++;
	i += 2;
	ft_putstr_fd("0x", 1);
	ft_printhex(address);
	return (i);
}
