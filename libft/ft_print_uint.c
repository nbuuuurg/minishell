/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_uint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:06:05 by nburgevi          #+#    #+#             */
/*   Updated: 2025/05/26 17:27:33 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putuint_fd(unsigned int n, int fd)
{
	char	c;

	if (n > 9)
		ft_putuint_fd((n / 10), fd);
	c = n % 10 + '0';
	ft_putchar_fd(c, fd);
}

int	print_uint(char format, unsigned long arg)
{
	int		r;
	char	*hex;

	r = 1;
	if (format == 'u')
		ft_putuint_fd((unsigned int)arg, 1);
	else
	{
		hex = ft_itoa_hex(format, arg);
		r = 0;
		while (hex[r])
			r++;
		ft_printhex(hex);
		return (r);
	}
	while (arg > 9)
	{
		arg = arg / 10;
		r++;
	}
	return (r);
}
